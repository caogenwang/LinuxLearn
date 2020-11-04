#!bin/bash
echo "shell script start"
var=1
var=$(($var+4))
# echo $var
echo "-----------"
# echo $((10>3))
expr $((6*8))
exit 10

