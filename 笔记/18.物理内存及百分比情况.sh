1）如何查看某个服务进程所占本机物理内存及百分比情况？可以使用Linux命令行查看，也可以使用脚本实现。如下以查看本机kube-nginx服务进程为例：

    [root@kevin ~]# pidof kube-nginx                
4763 4762
  
查看上面两个pid的物理内存使用情况
[root@kevin ~]# cat /proc/4763/status | grep VmRSS   
VmRSS:      4596 kB
[root@kevin ~]# cat /proc/4762/status|grep VmRSS
VmRSS:       224 kB
 
由上面可知，kube-nginx服务进程的两个pid所占物理内存为"4596+224=4820k"
  
编写python脚本
[root@kevin ~]# cat mem_percent.py
#!/usr/bin/python
#coding=utf8
import subprocess
pidcheck = subprocess.Popen('pidof kube-nginx',shell=True,stdout=subprocess.PIPE)
pid = pidcheck.stdout.read().rstrip().split()
memsize = 0
for i in pid:
   filename = "/proc/"+i+"/status"
   #os.path.join("/proc","i","status")     #等同于上条语句
   with open(filename,"r") as statusfile:
       for line in statusfile:
           if line.startswith("VmRSS"):
               memsize+=int(line.split()[1])
print "kube-nginx进程占用内存：%sk" % memsize
  
with open("/proc/meminfo","r") as memfile:
   for line in memfile:
       if line.startswith("MemTotal"):
           memtotal = int(line.split()[1])
print "kube-nginx进程内存占用率为：%.2f%%" % (memsize/float(memtotal)*100)
  
执行脚本：
[root@kevin ~]# chmod 755 mem_percent.py
  
[root@kevin ~]# ./mem_percent.py
kube-nginx进程占用内存：4820k
kube-nginx进程内存占用率为：0.06%
  
[root@kevin ~]# python ./mem_percent.py
kube-nginx进程占用内存：4820k
kube-nginx进程内存占用率为：0.06%
  
  
================================================================================================================
再看一例： 查看docked服务进程所占物理内存情况
[root@kevin ~]# pidof dockerd       
29496
[root@kevin ~]# cat /proc/29496/status|grep VmRSS
VmRSS:     78288 kB
  
[root@kevin ~]# cat mem1_percent.py
#!/usr/bin/python
#coding=utf8
import subprocess
pidcheck = subprocess.Popen('pidof dockerd',shell=True,stdout=subprocess.PIPE)
pid = pidcheck.stdout.read().rstrip().split()
memsize = 0
for i in pid:
   filename = "/proc/"+i+"/status"
   #os.path.join("/proc","i","status")     #等同于上条语句
   with open(filename,"r") as statusfile:
       for line in statusfile:
           if line.startswith("VmRSS"):
               memsize+=int(line.split()[1])
print "dockerd进程占用内存：%sk" % memsize
  
with open("/proc/meminfo","r") as memfile:
   for line in memfile:
       if line.startswith("MemTotal"):
           memtotal = int(line.split()[1])
print "dockerd进程内存占用率为：%.2f%%" % (memsize/float(memtotal)*100)
  
[root@kevin ~]# python mem1_percent.py
dockerd进程占用内存：78288k
dockerd进程内存占用率为：0.96%