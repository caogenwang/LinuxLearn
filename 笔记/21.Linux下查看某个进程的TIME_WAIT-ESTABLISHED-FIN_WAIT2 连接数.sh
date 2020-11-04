查看总的TIME_WAIT、ESTABLISHED、FIN_WAIT2 连接数 （下面两个命令都可以）
[root@kevin ~]# netstat -ae|grep "TIME_WAIT" |wc -l
[root@kevin ~]# ss -a|grep "TIME_WAIT" |wc -l
 
[root@kevin ~]# netstat -ae|grep "ESTABLISHED" |wc -l
[root@kevin ~]# netstat -ae|grep "FIN_WAIT2" |wc -l
 
  
比如查看kube-apiserve进程的TIME_WAIT、ESTABLISHED、FIN_WAIT2 连接数
[root@kevin ~]# netstat -antp | grep kube-apiserve | grep ESTABLISHED  -c4
85
[root@kevin ~]# netstat -antp | grep kube-apiserve | grep TIME_WAIT -c4  
0
[root@kevin ~]# netstat -antp | grep kube-apiserve | grep ESTABLISHED -c4
85
[root@kevin ~]# netstat -antp | grep kube-apiserve | grep FIN_WAIT2 -c4 
0
 
 
还可以根据某一个进程的端口号来查看
[root@kevin ~]# netstat -antp | grep 6443 | grep TIME_WAIT -c4          
0
[root@kevin ~]# netstat -antp | grep 6443 | grep ESTABLISHED -c4               #即ESTABLISHED连接数为8
8
[root@kevin ~]# netstat -antp | grep 6443 | grep FIN_WAIT2 -c4          
0
 
====================================================================================
Linux下查看并发连接数
 
[root@kevin ~]# netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
ESTABLISHED 218
 
或者可以简单的直接用：
[root@kevin ~]# netstat -n|grep "ESTABLISHED"|wc -l
218
 
解释：
/^tcp/    :表示搜索以tcp开头的行
S[$NF]    :表示以$NF为数组下标，建立数组S
++S[$NF]  :表示给数组S赋值，相当于有一个tcp那么这个S数组里的元素就+1
/^tcp/ {++S[$NF]}   :表示整个意思应该是 匹配出一个tcp协议的连接 数组S就+一个元素值
END是前面的/^tcp/ {++S[$NF]}  :表示执行结束后在执行
{for(a in S) print a, S[a]}   :表示将S值代入a 打印a 和这个S[a]数组的值。比如S是10，那么a就是10，S[a] 就是s这个数组的第10个元素的值
 
 
返回结果示例：
LAST_ACK 5
SYN_RECV 30
ESTABLISHED 1597
FIN_WAIT1 51
FIN_WAIT2 504
TIME_WAIT 1057
其中的SYN_RECV表示正在等待处理的请求数；ESTABLISHED表示正常数据传输状态；TIME_WAIT表示处理完毕，等待超时结束的请求数。
 
状态：描述
CLOSED：无连接是活动的或正在进行
LISTEN：服务器在等待进入呼叫
SYN_RECV：一个连接请求已经到达，等待确认
SYN_SENT：应用已经开始，打开一个连接
ESTABLISHED：正常数据传输状态
FIN_WAIT1：应用说它已经完成
FIN_WAIT2：另一边已同意释放
ITMED_WAIT：等待所有分组死掉
CLOSING：两边同时尝试关闭
TIME_WAIT：另一边已初始化一个释放
LAST_ACK：等待所有分组死掉
 
例子
查看Nginx运行进程数
[root@kevin ~]# ps -ef | grep nginx | wc -l      #返回的数字就是nginx的运行进程数
 
查看Web服务器进程连接数：
[root@kevin ~]# netstat -antp | grep 80 | grep ESTABLISHED -c4
 
查看MySQL和java进程连接数：
[root@kevin ~]# ps -axef | grep mysqld -c
1
[root@kevin ~]# ps -axef | grep java -c
3
 
注意：
grep -c 的作用类似grep | wc -l，不同的是，如果是查找多个文件，grep -c会统计每个文件匹配的行数，每行一个文件的列出来，
而wc -l 则列出总的统计数字。
另外grep -c 要比 grep | wc -l快一点。
 
 
====================================================================================
针对TIME_WAIT现象的优化措施：
  
比如某一天下午，登陆IDC机房的一台服务器，输入命令"netstat -an|grep mysql"，
发现mysql服务进程存在大量TIME_WAIT状态的连接
[root@kevin ~]# netstat -an|grep mysql
...........
tcp        0      0 127.0.0.1:3306              127.0.0.1:41378             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:41379             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:39352             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:39350             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:35763             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:39372             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:39373             TIME_WAIT
tcp        0      0 127.0.0.1:3306              127.0.0.1:41176             TIME_WAIT
...........
  
通过调整内核参数解决, 编辑文件，加入以下内容：
[root@kevin ~]# vim /etc/sysctl.conf
...........
net.ipv4.tcp_syncookies = 1
net.ipv4.tcp_tw_reuse = 1
net.ipv4.tcp_tw_recycle = 1
net.ipv4.tcp_fin_timeout = 30
   
然后让添加的参数生效。
[root@kevin ~]# /sbin/sysctl -p
  
其中：
net.ipv4.tcp_syncookies = 1  表示开启SYN Cookies。当出现SYN等待队列溢出时，启用cookies来处理，可防范少量SYN攻击，默认为0，表示关闭；
net.ipv4.tcp_tw_reuse = 1    表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；
net.ipv4.tcp_tw_recycle = 1  表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。
net.ipv4.tcp_fin_timeout=30  表示修改系統默认的TIMEOUT时间为30秒
  
修改之后，再用命令查看TIME_WAIT连接数
[root@kevin ~]# netstat -ae|grep "TIME_WAIT" |wc –l
  
此时发现大量的TIME_WAIT 已不存在，mysql进程的占用率很快就降下来的，网站访问正常。
不过很多时候，出现大量的TIME_WAIT状态的连接，往往是因为网站程序代码中没有使用mysql.colse()，才导致大量的mysql  TIME_WAIT.