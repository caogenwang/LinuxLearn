#!bin/bash

factoria(){
    local i=$1
    local f
    declare -i i
    declare -i f

    [ $i -le 2 ] && echo $i || { f=$((i-1));f=$(factoria $f);f=$((f*i));echo $f;}
}

[ $# -eq 0 ] && { echo "Usage:$0 number";exit; }
factoria $1