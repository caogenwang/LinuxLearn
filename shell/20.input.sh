#!bin/bash
if [ $# -lt 1 ];then
    echo "Usage: $0 directory..."
    exit
fi

file=$1
{
    read line1
    read line2
}< $file

echo "First line in $file is: "
echo "$line1"
echo "Second line in $file is: "
echo "$line2"

exit 0