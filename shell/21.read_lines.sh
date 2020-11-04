#!bin/bash
if [ $# -lt 1 ];then
    echo "Usage: $0 directory..."
    exit
fi

filename=$1
count=0
# while read LINE
until ! read LINE
do
    let count++
    echo "$count $LINE"
done < $filename
#没有指定的文件的话会从标准输入
echo -e "\Total $count lines read."
exit 0