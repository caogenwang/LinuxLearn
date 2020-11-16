#!bin/bash

netsub="192.168.2."

for ip in {2..254}
do (
    ping -c1 $netsub$ip &>/dev/null
    if [ $? -eq 0  ];then
        echo "$netsub$ip is open"
    else
        echo "$netsub$ip is close"
    fi
    )&
done