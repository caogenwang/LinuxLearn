CurrentDir=$0
CurrentDir="$( cd "$( dirname $CurrentDir  )" && pwd  )"
echo "CurrentDir=$CurrentDir"

Local_IP="$( /sbin/ifconfig -a|grep inet|grep -i 192.168|grep -v inet6|awk '{print $2}'|tr -d "addr:" )"
echo "Local_IP=$Local_IP"

docker stop registry-srv
docker rm registry-srv 
docker run -it \
--ulimit core=0 \
--log-driver none \
--name registry-srv \
-p 5000:5000 \
-v $CurrentDir/registry:/var/lib/registry \
-v $CurrentDir/config.yml:/etc/docker/config.yml \
registry \
# systemctl daemon-reload &&
# systemctl restart docker.service &&
# systemctl enable docker.service