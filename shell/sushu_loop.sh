#!bin/bash

read -p "Please input a num: " number

while [ $number != 'Q' ]
do
    echo $number
    read -p "Please input a num: " number
done

