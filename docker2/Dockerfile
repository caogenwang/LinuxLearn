version: '3.7'　　　　　　# docker stack 需要是3.0以上版本
services:
  registry-srv:　　　　　　# 服务名
    image: registry
    
    ports:　　　　　　　　　# 映射端口
      - 5000:5000
      
    volumes:　　　　　　　　# 挂载镜像路径和配置文件，注意修改路径与实际一致
      - /opt/zwx-registry:/var/lib/registry
      - /opt/zwx-registry/srv-config.yml:/etc/docker/registry/config.yml
      
    deploy:　　　　　　　　# 设置单任务，并约束主节点运行
      mode: replicated
      replicas: 1
      placement:
        constraints:
          - node.role == manager
      
  registry-web:　　　　　　# 服务名　　
    image: hyper/docker-registry-web
    
    ports:　　　　　　　　# 映射端口
      - 8000:8080
    
    volumes:　　　　　　# 挂载配置文件，注意修改路径与实际一致
      - /opt/zwx-registry/web-config.yml:/conf/config.yml
      
    environment:
      - REGISTRY_URL=http://registry-srv:5000/v2
      - REGISTRY_NAME=localhost:5000
    
    deploy:　　　　　　　　# 设置单任务，并约束主节点运行
      mode: replicated
      replicas: 1
      placement:
        constraints:
          - node.role == manager