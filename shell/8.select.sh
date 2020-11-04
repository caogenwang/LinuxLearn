#!bin/bash
PS3="Run command: "

select choice in date 2 hostname "uname -a" Exit
do
    case $chose in
        date)
            echo "Current system date and time:"
            $choice
            ;;
        w)
            echo "who is logging on...:"
            $choice
            ;;
        hostname)
            echo "Hostname:"
            $choice
            ;;
        "uname -a")
            echo "System information:"
            $choice
            ;;
        Exit)
        echo "Bye!"
        exit
        ;;
    esac
done