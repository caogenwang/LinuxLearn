package zookeeper

func (p*ZooKeeperRegisterPlugin) Start() error{

	if p.kv == nil{
		kv,err := libkv.NewStore(store.ZK,p.ZooKeeperServers,p.Options)
		if err != nil {
			log.Errorf("connot create zk registry:%v",err)
			return err
		}
		p.kv = kv
	}
	//在zookeeper上创建basepath地址：/rpcx/test
	err := p.kv.Put(p.BasePath,[]byte("rpcx_path"),&store.WriteOptions(IsDir: true))
	if err := nil {
		log.Errorf("cannot create zk path %s: %v", p.BasePath, err)
		return err;
	}
	//保存tps信息
   	//更新TTL（声明周期
	if p.UpdateInterval > 0{
		ticker := time.NewTicker(p.UpdateInterval)
		go func() {
			defer p.kv.Close()

			for(){
				select {
				case <-p.dying:
					close(p.done)
					return
				case <-ticker.C:
					var data []byte
					if p>metrics != nil {
						clientMeter := metrics.GetOrRegisterMeter("clientMeter",p.Metrics)
						data = []byte(strconv.FormatInt(clientMeter.Count()/60,10))
					}

					for _,name := range p.Services{
						nodePath := fmt.Sprintf("%s/%s/%s", p.BasePath, name, p.ServiceAddress)
						kvPaire,err := p.kv.Get(nodePath)
						if err != nil {
							log.Infof("can't get data of node: %s, because of %v", nodePath, err.Error())

							p.metasLock.RLock()
							meta := p.metas[name]
							p.metasLock.RUnlock()
							err = p.kv.Put(nodePath, []byte(meta), &store.WriteOptions{TTL: p.UpdateInterval * 2})
							if err != nil {
								log.Errorf("cannot re-create zookeeper path %s: %v", nodePath, err)
							}
						}
						else{
							//更新tps，设置目录的ttl
							v, _ := url.ParseQuery(string(kvPaire.Value))
							v.Set("tps", string(data))
							p.kv.Put(nodePath, []byte(v.Encode()), &store.WriteOptions{TTL: p.UpdateInterval * 2})
						}
					}
				}
			}
		}
	}
}