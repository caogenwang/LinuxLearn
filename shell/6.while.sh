#!bin/bash
# var=1
# while [ $var -le 4 ]
# do
#     echo "The for loop is run $var times"
#     var=$((var+1))
# done

# file=$1

# if [ $# -lt 1 ];then
#     echo "Usage: $0 FILEPATH"
#     exit
# fi

# while read -r f1 f2 f3
# do
#     echo $f1;echo $f2;echo $f2
# done < "$file"


# while true
# do
#     echo "Do something alway...."
#     sleep 1
# done

while :
do
    clear

    echo "===================="
    echo "      MAIN_MENU     "
    echo "===================="
    echo "1. Display date and time."
    echo "2. Display system information."
    echo "3. Display what useers are doing."
    echo "4. Exit"

    read -p "Enter your choice [ 1-4 ]: " choice

    case $choice in 
    1)
        echo "Today is $(date +%Y-%m-%d)."
        echo "Current time: $(date +%H:%M:%S)"
        read -p "Please [Enter] key to continue...." readEnterkey
        ;;
    2)
        uname -a
        read -p "Please [Enter] key to continue...." readEnterkey
        ;;
    3)
        w
        read -p "Please [Enter] key to continue...." readEnterkey
        ;;
    4)
        echo "Bye!"
        exit 0
        ;;
    *)
        echo "Error: Invalid option!"
        read -p "Please [Enter] key to continue...." readEnterkey
        ;;
    esac
done


