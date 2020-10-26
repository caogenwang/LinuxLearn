#!/bin/bash
PATH=/bin:/usr/bin:/sbin:/usr/sbin:/usr/local/bin:/usr/local/sbin;
export PATH
function traffic_monitor {
 
  OS_NAME=$(sed -n '1p' /etc/issue)
 
  eth=$1
 
  if [ ! -d /sys/class/net/$eth ];then
      echo -e "Network-Interface Not Found"
      echo -e "You system have network-interface:\n`ls /sys/class/net`"
      exit 5
  fi
  while [ "1" ]
  do
    
    STATUS="fine"
   
    RXpre=$(cat /proc/net/dev | grep $eth | tr : " " | awk '{print $2}')
    TXpre=$(cat /proc/net/dev | grep $eth | tr : " " | awk '{print $10}')
 
    sleep 1
    RXnext=$(cat /proc/net/dev | grep $eth | tr : " " | awk '{print $2}')
    TXnext=$(cat /proc/net/dev | grep $eth | tr : " " | awk '{print $10}')
    clear
  
    RX=$((${RXnext}-${RXpre}))
    TX=$((${TXnext}-${TXpre}))
 
    if [[ $RX -lt 1024 ]];then
      RX="${RX}B/s"
    elif [[ $RX -gt 1048576 ]];then
      RX=$(echo $RX | awk '{print $1/1048576 "MB/s"}')
      $STATUS="busy"
    else
      RX=$(echo $RX | awk '{print $1/1024 "KB/s"}')
    fi
 
    if [[ $TX -lt 1024 ]];then
      TX="${TX}B/s"
      elif [[ $TX -gt 1048576 ]];then
      TX=$(echo $TX | awk '{print $1/1048576 "MB/s"}')
    else
      TX=$(echo $TX | awk '{print $1/1024 "KB/s"}')
    fi
   
    echo -e "==================================="
    echo -e "Welcome to Traffic_Monitor stage"
    echo -e "version 1.0"
    echo -e "Since 2018.7.2"
    echo -e "Created by wangshibo"
    echo -e "BLOG: http://www.cnblogs.cn/kevingrace"
    echo -e "==================================="
    echo -e "System: $OS_NAME"
    echo -e "Date:   `date +%F`"
    echo -e "Time:   `date +%k:%M:%S`"
    echo -e "Port:   $1"
    echo -e "Status: $STATUS"
    echo -e  " \t     RX \tTX"
    echo "------------------------------"
    
    echo -e "$eth \t $RX   $TX "
    echo "------------------------------"
 
    echo -e "Press 'Ctrl+C' to exit"
  done
}
 
if [[ -n "$1" ]];then
  traffic_monitor $1
else
  echo -e "None parameter,please add system netport after run the script! \nExample: 'sh traffic_monitor eth0'"
fi