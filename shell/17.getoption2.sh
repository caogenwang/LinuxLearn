#!bin/bash
vflag=off
filename=""
output=""

function usage(){
    echo "Usge:"
    echo " myscript [-h] [-v] [-f <filename>] [-o <filename>]"
    exit -1
}

while getopts :hvf:o: opt
do
    case "$opt" in 
    v)
        vflag=on
    ;;
    f)
        filename=$OPTARG
        if [ ! -f $filename ]
        then
            echo "The source file $filename doesn't exist!"
            exit 
        fi
    ;;
    o)
        output=$OPTARG
        if [ ! -d 'dirname $output' ]
        then
            echo "The output path 'dirname $output' doesn't exist!"
            exit
        fi
    ;;
    h)
        usage
        exit
    ;;
    :)
        echo "The option -$OPTARG requires an argument."
        exit
    ;;
    ?)
        echo "Invalid option:-${OPTARG}"
        usage
        exit 2
    ;;
    esac
done