CurrentDir=$0
CurrentDir="$( cd "$( dirname $CurrentDir  )" && pwd  )"
echo "CurrentDir=$CurrentDir"

mkdir -p $CurrentDir/../_build/mac
cd $CurrentDir/../_build/mac && cmake -GXcode $CurrentDir/../

open $CurrentDir/../_build/mac/mytbf.xcodeproj
