#!bin/bash
# passed(){
#     a=$1
#     echo "passed():\$0 is $0"
#     echo "passed():\$1 is $1"
#     echo "passed():\$a is $a"
#     echo "passed():\$1 is $2"
#     echo "passed():\$1 is $3"
#     echo "passed():total args passed to me - \"$@\""
#     echo "passed():all args (\$*) passed to me - \"$*\""
# }
# echo "outside arg passed"
# echo $1
# echo $2
# echo "***calling passed() first time***"
# passed one
# echo "***calling passed() second time***"
# passed one two three
#命令行传入的参数会被函数中传入的参数给覆盖

#加载函数文件
. /Users/yangsen/work/LinuxLearn/shell/function.sh

is_root && echo "You are logged in as root" || echo "You are not logged as root"