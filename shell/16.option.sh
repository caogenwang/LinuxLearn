#!bin/bash
opt=$1
filename=$2

checkfile()
{
    if [ -z $filename ]
    then
        echo "File name missing"
    elif [ ! -f $filename ]
    then
        echo "The file $filename doesn't exist!"
        exit 2
    fi
}

case $opt in
    -e|E)
        checkfile
        echo "Editing $filename file..."
    ;;
    -p|P)
        checkfile
        echo "Displaying $filename file..."
    ;;
    *)
        echo "Bad args"
    ;;
esac
