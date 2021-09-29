# linux 端口耗尽,解决端口耗尽问题: tcp_tw_reuse、tcp_timestamps

一、本地端口有哪些可用

首先，需要了解到TCP协议中确定一条TCP连接有4要素：local IP, local PORT, remote IP, remote PORT。这个四元组应该是唯一的。

在我们发送HTTP请求的时候，local IP remote IP remote PORT是固定的，只有local PORT是可变的，可用的local PORT的数量就限制了client和server之间TCP连接数的数量。

TCP协议中PORT部分是用两个字节来表示的，也就是说可用的端口数量肯定不能超过65536个。

sysctl -a|grepnet.ipv4.ip_local_port_range

net.ipv4.ip_local_port_range= 32768 61000

表示client可用的端口是[32768, 61000]，共28233个。那么这台机器和另外任意一台机器，同时只能建立28233个TCP连接。

可打开的最大文件数

ulimit -a

corefile size (blocks, -c) 0data seg size (kbytes,-d) unlimited

scheduling priority (-e) 0

file size (blocks, -f) unlimited

pending signals (-i) 15088max locked memory (kbytes,-l) 64max memory size (kbytes,-m) unlimited

open files (-n) 65535pipe size (512 bytes, -p) 8POSIX message queues (bytes,-q) 819200real-time priority (-r) 0stack size (kbytes,-s) 8192cputime (seconds, -t) unlimited

max user processes (-u) 4096virtual memory (kbytes,-v) unlimitedfile locks (-x) unlimited

/etc/security/limits.conf

\#Where:

\#can be:

\#-a user name

\#-a group name, with @group syntax

\#- the wildcard *, fordefault entry

\#- the wildcard %, can be also used with %group syntax,

\#formaxlogin limit

\#

\#can have the two values:

\#- "soft" forenforcing the soft limits

\#- "hard" forenforcing hard limits

\#

\#can be one of the following:

\#- core - limits the core filesize (KB)

\#- data -max data size (KB)

\#- fsize -maximum filesize (KB)

\#- memlock - max locked-in-memory address space (KB)

\#- nofile - max number of open filedescriptors

\#- rss -max resident set size (KB)

\#- stack -max stack size (KB)

\#- cpu - max CPU time(MIN)

\#- nproc -max number of processes

\#- as -address space limit (KB)

\#- maxlogins - max number of logins forthis user

\#- maxsyslogins -max number of logins on the system

\#- priority -the priority to run user process with

\#- locks - max number of filelocks the user can hold

\#- sigpending -max number of pending signals

\#- msgqueue -max memory used by POSIX message queues (bytes)

\#- nice - max nice priority allowed to raise to values: [-20, 19]

\#- rtprio -max realtime priority

\#

\##

\#* soft core 0#* hard rss 10000#@student hard nproc20#@faculty soft nproc20#@faculty hard nproc50#ftp hard nproc 0#@student- maxlogins 4# End offileroot soft nofile65535root hard nofile65535

\* soft nofile 65535

\* hard nofile 65535

二、短连接并不会同时存在大量TCP连接，端口为什么还是耗尽了？

上一步我们分析到，client和server之间只能同时存在28233个TCP连接，但是我们的压测用的是短连接，连接用完就释放掉了，端口应该也会释放掉，为啥还会产生端口耗尽的问题呢？

这就需要提到TIME_WAIT这个状态了，TCP连接断开的时候，主动发起连接断开操作的一方，最后会停留在TIME_WAIT状态，会持续2*MSL的时长，

这个状态的端口是不能被使用的，准确的说是当新的TCP连接的local IP remote IP和remote PORT和TIME_WAIT状态的连接一致时这个端口不能被使用。

sysctl -a|grepnet.ipv4.tcp_fin_timeout

net.ipv4.tcp_fin_timeout= 60

可以推论：

如果client机器有28233端口可用，TIME_WAIT 60秒，短连接的方式发起请求，那么这个client发起的请求的QPS是不能超过28233/60的。

三、为什么有TIME_WAIT状态

想象这么一个场景：

A和B建立了一个TCP连接，A向B发送消息包1 2 3，消息包3传给B的时候延迟了，A又重传了消息包3，A和B完成通信断开连接，双方都很happy。

\2. A和B又建立一个TCP连接，用了相同的local IPlocal PORTremote IPremote PORT，A向B发送消息包1 2，B收到1 2之后

上一个连接延迟的消息包3来了，B无法区分这个消息包是上一个TCP连接的

TIME_WAIT状态主要就是为了解决这个问题：防止延迟的无效消息包被误认为是合法的。

TIME_WAIT状态持续2倍MSL之后，可以确保老的TCP连接逗留在网络上的消息包已经全部消失了。

四、单台机器发送请求每秒要超过(可用端口数/tcp_fin_timeout)怎么办

使用长连接或者打开tcp_tw_reuse。

那么为什么打开tcp_tw_reuse就可以使用TIME_WAIT状态的端口呢？不怕延迟的消息包被误认为合法的么？

这里就需要提到另外一个选项了：tcp_timestamps，打开这个选项之后，TCP包里面会带上发包机器的当前时间戳。

可以这么想象一下：如果每个TCP包带上原始发包时的时间戳，如果一个延迟的消息包到达B，B可以拿这个消息包的时间和TCP连接建立的时间做一个对比，

如果消息包的时间比较早，那么这就是上一个TCP连接延迟的消息包，丢弃掉就好。

通过时间戳的比较，也可以解决延迟消息包被误认为合法的问题。因此同时打开tcp_timestamps和tcp_tw_reuse之后，client端的TIME_WAIT端口是可以被复用的。

但要注意的是：

\1. tcp_timestamps需要client和server端同时打开才生效(我们这次遇到端口耗尽的问题就是因为server端没有打开tcp_timestamps选项)

\2. tcp_timestamps精确到秒，也就是TIME_WAIT状态的端口也在下一秒才可以重用，如果继续使用短连接的话，发送请求的QPS是不能超过可用端口数的

