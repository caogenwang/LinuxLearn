#!bin/bash
if [ $# -lt 1 ];then
    echo "Usage: $0 directory..."
    exit
fi

for dir in $@
do
    find $dir -name "*.tmp" -exec rm -f {} \;
done 2> error.log