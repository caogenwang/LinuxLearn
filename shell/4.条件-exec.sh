#!bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: 'basename $0' Number"
fi

num=$1

if [[ $num -ge 90 && $num -le 100 ]]
then
    echo "Excellent!"
elif [[ $num -ge 80 && $num -lt 90 ]]
then
    echo "Good!"
elif [[ $num -ge 60 && $num -lt 80 ]]
then
    echo "Pass mark!"
elif [[ $num -lt 60 && $num -ge 0 ]]
then
    echo "Fail!"
else
    echo "Wrong number!"
fi