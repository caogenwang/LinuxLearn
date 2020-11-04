#!bin/bash
#正则表达式

if [ $# != 1 ];then

    echo "Usage: $0 address"
else
    ip=$1
fi

if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]];then
    echo "Looks like an IPv4 IP address."
elif [[ $ip =~ ^[A-Fa-f0-9:]+$ ]];then
    echo "Could be an IPv6 address."
else
    echo 'Oops!'
fi