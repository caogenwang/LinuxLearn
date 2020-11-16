#!bin/bash

number=100
read -p "Please guess the num: " input

while [ $input != $number ]
do
    if [ $input -lt $number ];then
        echo "input is smaller,continue:"
    elif [ $input -gt $number ];then
        echo "input is bigger,continue:"
    else
        break
    fi
    read input
done

echo "bingo!"