使用Linux命令行直接某个进程的CPU百分比和内存百分比

使用"ps auxw" 可以查看到本机的进程所占cpu和mem的百分比情况
[root@m7-autocv-gpu02 ~]# ps auxw|head -1
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
 
%CPU 进程的cpu占用率
%MEM 进程的内存占用率
 
查看java进程所占本机的cpu百分比， 如下为0.3%
[root@m7-autocv-gpu02 ~]# ps auxw |grep -v grep|grep -w java|awk '{print $3}'          
0.3
 
查看java进程所占本机的内存百分比， 如下为18.0%  （这个值跟上面python脚本查出来的18.05%稍微有点误差，这里百分比的数只精确到小数点后一位）
[root@m7-autocv-gpu02 ~]# ps auxw |grep -v grep|grep -w java|awk '{print $4}'
18.0

Linux下查看哪些进程占用的CPU和内存资源最多的方法

1）获取占用CPU资源最多的10个进程
[root@kevin ~]# ps aux|head -1;ps aux|grep -v PID|sort -rn -k +3|head -10
 
2）获取占用内存资源最多的10个进程
[root@kevin ~]# ps aux|head -1;ps aux|grep -v PID|sort -rn -k +4|head -10
 
3）查看占用cpu最高的进程
[root@kevin ~]# ps aux|head -1;ps aux|grep -v PID|sort -rn -k +3|head -1
 
4）获取占用内存资源最高的进程
[root@kevin ~]# ps aux|head -1;ps aux|grep -v PID|sort -rn -k +4|head -1
