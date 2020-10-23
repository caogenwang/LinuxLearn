CurrentDir=$0
CurrentDir="$( cd "$( dirname $CurrentDir  )" && pwd  )"

mkdir -p $CurrentDir/../_build/linux
cd $CurrentDir/../_build/linux
cmake ../..
make
