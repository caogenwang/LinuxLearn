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