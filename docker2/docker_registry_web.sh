CurrentDir=$0
CurrentDir="$( cd "$( dirname $CurrentDir  )" && pwd  )"
echo "CurrentDir=$CurrentDir"

Local_IP="$( /sbin/ifconfig -a|grep inet|grep -i 192.168|grep -v inet6|awk '{print $2}'|tr -d "addr:" )"
echo "Local_IP=$Local_IP"

docker stop registry-web
docker rm registry-web
docker run -it \
--ulimit core=0 \
--log-driver none \
--name registry-web \
-p 5001:5001 \
-v $CurrentDir/web-config.yml:/etc/lib/config.yml \
hyper/docker-registry-web