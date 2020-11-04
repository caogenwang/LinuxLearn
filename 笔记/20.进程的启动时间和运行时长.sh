比如查看java进程的启动时间和运行时长
[root@m7-autocv-gpu02 ~]# ps -ef|grep -v grep|grep -w java|awk '{print $2}'
18661
 
[root@m7-autocv-gpu02 ~]# ps -eo pid,lstart,etime | grep 18661            
18661 Tue May 14 19:22:51 2019  5-19:42:51
 
其中：
Tue May 14 19:22:51 2019   为java进程的启动时间
5-19:42:51                 为java进程的运行时长，即5天19小时42分51秒
 
 
可以查看所有进程的启动事件、运行时长
[root@m7-autocv-gpu02 ~]# ps -eo user,pid,lstart,etime,cmd
 
查看nginx进程启动的精确时间和启动后运行的时长
[root@m7-autocv-gpu02 ~]# ps -eo pid,lstart,etime,cmd|grep nginx
