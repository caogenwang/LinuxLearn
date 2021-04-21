安装私人docker
1. 先将配置文件主备好
2. 执行docker脚本，记得将对应的配置文件的存储位置映射好
3. 修改daemon中的仓库地址
4. 修改镜像的tag：docker tag IP:PORT/image,docker push IP:PORT/image 
5. 查看仓库镜像
    curl -XGET http://xx.xx.xx.xx:5000/v2/_catalog　　　　# 查看仓库镜像列表（也可以通过windows浏览器打开查看）
    curl -XGET http://xx.xx.xx.xx:5000/v2/image_name/tags/list　　# 查看指定应用镜像tag

1. https://www.cnblogs.com/leozhanggg/p/12050322.html
2. https://www.cnblogs.com/leozhanggg/p/12040338.html
3. https://www.cnblogs.com/leozhanggg/p/12039953.html

