#!bin/bash
if [ $# -lt 1 ];then
    echo "Usage: $0 directory..."
    exit
fi

exec 3< $1

while read -u 3 line
do 
    echo $line
    read -p "Press any key" -n 1
done

# exec 3<&-

#将文件也重定向到标准输入，也就是read都要读取标准输入。
#如果不做重定向，while循环读取的是文件，循环内的read也会读取文件，不会等待
#屏幕的输入
