#!bin/bash
#函数的后台运行

progress(){
    echo -n "$0: Please wait..."
    while true
    do
        echo -n "."
        sleep 5
    done
}

dobackup(){
    echo "Do backup work"
}

progress &

MYSELF=$!

dobackup()

kill $MYSELF > /dev/null 2>&1
echo -n "...done"
echo