#!bin/bash
# while [ $# -ne 0 ]
while [ -n "$1" ]
do
    echo "Current Parameter: $1,Remaining $#."

    #将$1参数传递给函数使用

    shift 5
    if [ $? -ne 0 ]
    then 
        break;
    fi
done