#!bin/bash
while getopts ":a" opt
do
    case $opt in 
    a)
        echo "The option -a was triggered!"
    ;;
    \?)
        echo "Invalid option:-${OPTARG}"
        break;
    ;;
    esac
done


