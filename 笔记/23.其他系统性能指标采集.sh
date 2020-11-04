1）通过free命令查看内存剩余可用情况
==================================================================================
-b：以Byte为单位显示内存使用情况。
-k：以KB为单位显示内存使用情况。
-m：以MB为单位显示内存使用情况。
-g   以GB为单位显示内存使用情况。
-o：不显示缓冲区调节列。
-s<间隔秒数>：持续观察内存使用状况。
-t：显示内存总和列。
-V：显示版本信息。
-h：centos7系统下，可以使用该参数进行查看
 
[root@kevin ~]# free -m
              total        used        free      shared  buff/cache   available
Mem:           3774        1155         591         291        2027        1962
Swap:          2048         901        1147
 
页面显示参数解释：
total: 总计物理内存的大小。
used: 已使用多大。
free: 可用有多少。
Shared: 多个进程共享的内存总额。
Buffers/cached: 磁盘缓存的大小
Swap:交换分区(虚拟内存)情况；
Mem:实际内存的使用情况
 
2）查看和设置服务器最大进程数
==================================================================================
查看系统允许的当前最大进程数
查看本服务器进程数
[root@kevin ~]# ps -eLf | wc -l
263
 
主要是看pid_max指标的值，pid_max是Linux内核用来限制进程数的一个最大指标。
换言之，即最大允许进程就这么多，超过了这个设定值，服务器就挂了（"ps -eLf | wc -l"查看本机器的进程数）
[root@kevin ~]# sysctl kernel.pid_max    # centos7系统执行"systemctl kernel.pid_max"命令
kernel.pid_max = 32768
或者
[root@kevin ~]# cat /proc/sys/kernel/pid_max
32768
 
一般出现 -bash: fork: Cannot allocate memory 本机的进程数应该接近或等于pid_max指标值的。
[root@kevin ~]# echo 65536 > /proc/sys/kernel/pid_max                   # 临时修改
[root@kevin ~]# echo "kernel.pid_max=65536" >> /etc/sysctl.conf         # 永久修改
[root@kevin ~]# sysctl -p
kernel.pid_max = 65536
 
如上设置就可以了，保险起见，最好找到占用空间最大的进程，然后把它干掉就解决了！如下使用top命令分析：
 
3）top:展示进程视图，监控服务器进程数值默认进入top时，各进程是按照CPU的占用量来排序的
==================================================================================
[root@kevin ~]# top
top - 15:46:45 up 581 days, 22:30,  5 users,  load average: 0.00, 0.02, 0.05
Tasks: 133 total,   2 running, 130 sleeping,   1 stopped,   0 zombie
%Cpu(s):  0.3 us,  0.3 sy,  0.0 ni, 99.3 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  7994044 total,  1227680 free,  3499004 used,  3267360 buff/cache
KiB Swap:  2097148 total,  1708072 free,   389076 used.  3954372 avail Mem
 
   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND                                                                    
 48085 root      20   0 4172780 814536   6236 S   0.7 21.1   8353:58 mongod                                                                     
 12670 root      20   0  703816  41532    924 S   0.3  1.1 331:29.13 glusterfs                                                                  
 55185 root      20   0  157724   2240   1548 R   0.3  0.1   0:00.03 top                                                                        
     1 root      20   0   51656   2496   1352 S   0.0  0.1  94:14.96 systemd                                                                    
     2 root      20   0       0      0      0 S   0.0  0.0   0:01.87 kthreadd                                                                   
     3 root      20   0       0      0      0 S   0.0  0.0   2:32.08 ksoftirqd/0                                                                
     5 root       0 -20       0      0      0 S   0.0  0.0   0:00.00 kworker/0:0H                                                               
     7 root      rt   0       0      0      0 S   0.0  0.0   1:16.86 migration/0                                                                
     8 root      20   0       0      0      0 S   0.0  0.0   0:00.00 rcu_bh                                                                     
     9 root      20   0       0      0      0 R   0.0  0.0 358:31.04 rcu_sched                                                                  
    10 root      rt   0       0      0      0 S   0.0  0.0   5:39.75 watchdog/0                                                                 
    11 root      rt   0       0      0      0 S   0.0  0.0   4:54.62 watchdog/1                                                                 
    12 root      rt   0       0      0      0 S   0.0  0.0   1:17.38 migration/1                                                                
    13 root      20   0       0      0      0 S   0.0  0.0   2:26.26 ksoftirqd/1                                                                
    15 root       0 -20       0      0      0 S   0.0  0.0   0:00.00 kworker/1:0H                                                               
    17 root      20   0       0      0      0 S   0.0  0.0   0:00.00 kdevtmpfs                                                                  
    18 root       0 -20       0      0      0 S   0.0  0.0   0:00.00 netns                                                                      
    19 root      20   0       0      0      0 S   0.0  0.0   0:26.52 khungtaskd                                                                 
    20 root       0 -20       0      0      0 S   0.0  0.0   0:00.00 writeback    
 
针对以上的top命令显示的结果，逐行说明：
 
第一行：
15:46:45 — 当前系统时间
581 days, 22:30 — 系统已经运行了581天22小时30分钟（在这期间没有重启过）
5 users — 当前有5个用户登录系统
lload average: 0.00, 0.02, 0.05 — load average后面的三个数分别是1分钟、5分钟、15分钟的负载情况
 
这里需要注意：
load average数据是每隔5秒钟检查一次活跃的进程数，然后按特定算法计算出的数值。
如果这个数除以逻辑CPU的数量，结果高于5的时候就表明系统在超负荷运转了。
 
第二行：
Tasks — 任务（进程），系统现在共有133个进程，其中处于运行中的有2个，130个在休眠（sleep），stoped状态的有1个，zombie状态（僵尸）的有0个。
 
第三行：cpu状态
0.3% us — 用户空间占用CPU的百分比。
0.3% sy — 内核空间占用CPU的百分比。
0.0% ni — 改变过优先级的进程占用CPU的百分比
99.3% id — 空闲CPU百分比
0.0% wa — IO等待占用CPU的百分比
0.0% hi — 硬中断（Hardware IRQ）占用CPU的百分比
0.0% si — 软中断（Software Interrupts）占用CPU的百分比
0.0% st — 虚拟机偷取占用CPU的百分比
 
第四行：内存状态（k）
7994044 total — 物理内存总量（8GB）
1227680 free — 使用中的内存总量（12.2GB）
3499004 used — 空闲内存总量（350M）
3267360 buff/cache — 缓存的内存量 （326M）
 
第五行：swap交换分区（k）
2097148 tota — 交换区总量（2GB）
1708072 free — 空闲交换区总量（1.7G）
389076 used — 使用的交换区总量（390M）
3954372 avail Mem — 目前可用的内存总量（4GB）。剩余内存+剩余的swap内存
 
第六行以下：各进程（任务）的状态监控
PID — 进程id
USER — 进程所有者
PR — 进程优先级
NI — nice值。负值表示高优先级，正值表示低优先级
VIRT — 进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES
RES — 进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA
SHR — 共享内存大小，单位kb
S — 进程状态。D=不可中断的睡眠状态 R=运行 S=睡眠 T=跟踪/停止 Z=僵尸进程
%CPU — 上次更新到现在的CPU时间占用百分比
%MEM — 进程使用的物理内存百分比
TIME+ — 进程使用的CPU时间总计，单位1/100秒
COMMAND — 进程名称（命令名/命令行）
 
-------------------------------------
扩展：top命令的多U多核CPU监控
1. 在top基本视图中，按键盘数字"1"，可监控每个逻辑CPU的状况：
top - 18:04:41 up 85 days,  3:25,  3 users,  load average: 0.11, 0.13, 0.13
Tasks: 128 total,   2 running, 126 sleeping,   0 stopped,   0 zombie
%Cpu0  :  2.0 us,  0.7 sy,  0.0 ni, 97.0 id,  0.3 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu1  :  2.7 us,  0.7 sy,  0.0 ni, 96.6 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
%Cpu2  :  3.7 us,  0.0 sy,  0.0 ni, 96.0 id,  0.0 wa,  0.0 hi,  0.3 si,  0.0 st
%Cpu3  :  2.3 us,  0.3 sy,  0.0 ni, 97.3 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  7994044 total,  1179668 free,  3552276 used,  3262100 buff/cache
KiB Swap:  2097148 total,  1708072 free,   389076 used.  3900944 avail Mem
.......
.......
 
2. 敲击键盘"b", 可以"打开/关闭"top视图的加亮效果。
3. 敲击键盘"x", 可以"打开/关闭"top视图的排序列的加亮效果。
4. 如果要在基本视图中显示"CODE"和"DATA"两个字段，可以通过敲击"r"和"s"键。
 
4）Linux查看进程、CPU、内存、IO相关信息
==================================================================================
1. 监控java线程数：
# ps -eLf | grep java | wc -l
 
2. 监控网络客户连接数：
# netstat -n | grep tcp | grep 侦听端口 | wc -l
 
3. 进程相关
# cat /proc/your_PID/status 
通过"top"或"ps -ef |grep '进程名'" 得到进程的PID。然后执行上面命令可以提供进程状态、文件句柄数、内存使用情况等信息。
 
4. 内存相关
# vmstat -s -S M 
该可以查看包含内存每个项目的报告，通过-S M或-S k可以指定查看的单位，默认为kb。结合watch命令就可以看到动态变化的报告了。
也可用  cat /proc/meminfo 
 
5. 查看cpu的配置信息
# cat /proc/cpuinfo 
它能显示诸如CPU核心数，时钟频率、CPU型号等信息。
 
6. 要查看cpu波动情况的，尤其是多核机器上，可以使用下面命令：
# mpstat -P ALL 10
该命令可间隔10秒钟采样一次CPU的使用情况，每个核的情况都会显示出来，例如，每个核的idle情况等。
 
如果只是需查查看看均值的CPU，可以使用下面命令：
# iostat -c
 
7. IO相关
# iostat -P ALL 
该命令可查看所有设备使用率、读写字节数等信息。
 
5）Linux查看物理CPU个数、核数、逻辑CPU个数
==================================================================================
总核数 = 物理CPU个数 * 每颗物理CPU的核数
总逻辑CPU数 = 物理CPU个数 * 每颗物理CPU的核数 * 超线程数
 
1. 查看物理CPU个数
# cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l
 
2. 查看每个物理CPU中core的个数(即核数)
# cat /proc/cpuinfo| grep "cpu cores"| uniq
 
3. 查看逻辑CPU的个数
# cat /proc/cpuinfo| grep "processor"| wc -l
 
4. 查看CPU信息（型号）
# cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c