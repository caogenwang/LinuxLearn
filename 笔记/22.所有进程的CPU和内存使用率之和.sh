查看本机所有进程的CPU占比之和
[root@kevin ~]# cat cpu_per.sh
#!/bin/bash
 
ps auxw|awk '{if (NR>1){print $3}}' > /opt/cpu_list
 
awk '{CPU_PER+=$1}END{print CPU_PER}'  /opt/cpu_list
 
执行脚本，获取所有进程的cpu占比之和，即14.1%
[root@kevin ~]# sh cpu_per.sh
14.1
 
==========================================================
查看本机所有进程的内存占比之和
[root@kevin ~]# cat mem_per.sh
#!/bin/bash
 
ps auxw|awk '{if (NR>1){print $4}}' > /opt/mem_list
 
awk '{MEM_PER+=$1}END{print MEM_PER}'  /opt/mem_list
 
执行脚本，获取所有进程的内存占比之和，即22.2%
[root@kevin ~]# sh mem_per.sh
22.2
 
 
脚本配置解释：
ps -auxw|awk '{print $3}'   表示列出本机所有进程的cpu利用率情况，结果中第一行带"%CPU"字符
ps -auxw|awk '{print $4}'   表示列出本机所有进程的内存利用率情况，结果中第一行带"%MEM"字符
ps auxw|awk '{if (NR>1){print $4}}  表示将"ps auxw"结果中的第一行过滤(NR>1)掉，然后打印第4行

