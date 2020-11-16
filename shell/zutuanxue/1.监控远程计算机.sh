#!bin/bash

while :
do
    ping -c3 www.baidu.com >/dev/null 
    if [ $? -eq 0 ];then
        echo "OK!"
    else
        echo "NO!"
    fi
    sleep 5
done