#!bin/bash
var=1
until [ $var -gt 3 ]
do
    echo "The for loop is run $var times"
    var=$((var+1))
done