#!/bin/bash
#filename:Monitor_Disk
Monitor_Disk(){
mkdir -p /mnt/Monitor_Disk
fdisk -l|grep "Disk /dev/" | awk '{print $2,$3$4}'|tr -d ',:'>/mnt/Monitor_Disk/device_list.log
N=1;ECHO 90
while read device_line
do
Device=`echo $device_line|awk '{print $1}'`
Sum=`echo $device_line|awk '{print $2}'`
df -h |grep "$Device"|sort>/mnt/Monitor_Disk/${N}_partitions.log
echo
echo "** 第$N块硬盘($Device):${Sum} **"| grep -E "$Device|$Sum|$N" --color=yes
echo "------------------------------------"
echo -e "linux分区 挂载目录 总大小 已用 剩余 已用百分比 文件系统 ID system  \
块大小  预留空间  ">/mnt/Monitor_Disk/${N}_Over.log
echo -e "========= ======== =====  ===  ===  ==========  ======= == ======  \
======  ======== ">>/mnt/Monitor_Disk/${N}_Over.log
Num_Partition=`cat /mnt/Monitor_Disk/${N}_partitions.log|wc -l`
n=0
while read partition_line
do
    Partition_Name=`echo $partition_line|awk '{print $1}'`
    Mount_Dir=`echo $partition_line|awk '{print $6}'`
    Partition_Sum=`echo $partition_line|awk '{print $2}'`
    Partition_Used=`echo $partition_line|awk '{print $3}'`
    Partition_Leave=`echo $partition_line| awk '{print $4}'`
    Partition_Percent=`echo $partition_line|awk '{print $5}'`
    Partition_Type=`mount|grep $Partition_Name|awk '{print $5$6}'`
    Partition_Id=`fdisk -l | grep $Partition_Name|tr -d '\*'|awk '{print $5}'`
    Partition_System=`fdisk -l | grep $Partition_Name|tr -d '\*'|awk '{print $6}'`
    Part_Block_Size_B=`tune2fs -l $Partition_Name|grep "Block size"|awk '{print $3}'`
    Part_Lift_For_Root_Blocks=`tune2fs -l $Partition_Name|grep "Reserved block count:"|\
    awk '{print $4}'`
    Part_Block_Size=`echo $Part_Block_Size_B/1024|bc`
    Part_Lift_For_Root=`echo ${Part_Lift_For_Root_Blocks}*${Part_Block_Size}/1024|bc`
    echo -e "$Partition_Name $Mount_Dir $Partition_Sum $Partition_Used $Partition_Leave \
$Partition_Percent $Partition_Type  $Partition_Id $Partition_System  \
${Part_Block_Size}K ${Part_Lift_For_Root}M">>/mnt/Monitor_Disk/${N}_Over.log
    let n++
 
    [ $n -eq $Num_Partition ]&&(cat /mnt/Monitor_Disk/${N}_Over.log|column -t;echo)
done</mnt/Monitor_Disk/${N}_partitions.log
let N++
done</mnt/Monitor_Disk/device_list.log
ECHO 90
rm -fr /mnt/Monitor_Disk
}
ECHO(){
for ((i=1;i<=$1;i++))
do
echo -n "#"
[ $i -eq $1 ]&&(echo;echo)
done
}
Monitor_Disk