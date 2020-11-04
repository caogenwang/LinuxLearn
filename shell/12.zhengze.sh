#!bin/bash
#正则表达式

read -p "Input a number,please: " number

if [[ $number =~ ^[0-9]+$ ]];then
    echo "It's a number."
else
    echo "It's not a number."
fi