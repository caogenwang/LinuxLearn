#!bin/bash
E_BADARGS=65
if [ ! -n "$1" ]
then
    echo "Usage:'basename $0' arg1 arg2..."
    exit $E_BADARGS
fi

index=1
echo "Listing args with \$*: "

for arg in $*
do
    echo "Arg #$index = $arg"
    let index+=1
done

index=1
echo "Listing args with \"\$@\": "

for arg in "$@"
do
    echo "Arg #$index = $arg"
    let index+=1
done

echo "$*"
echo "$@"

