package center

import(
	"log"
)
type KVPair struct{

}

type ServiceDiscovery interface{
	GetServices() []*KVPair
	WatchService() chan []*KVPair
	RemoveWatcher(ch chan []*KVPair)
	Clone(servicePath string) ServiceDiscovery
	Close()
}

type ZookeeperDiscovry struct{
	pairs []*KVPair
	chans []chan []*KVPair
}

func (d *ZookeeperDiscovry) WatchService() chan[]*KVPair{
	ch := make(chan []*KVPair, 10)
	d.chans = append(d.chans,ch)
	return ch
}

func (d *ZookeeperDiscovry)watch()  {
	for {
		select {
		case <-d.stopCh:
			log.Info()
			return
		case ps := <-c:
			if ps != nil{
				break
			}
			var pairs []*KVPair // latest servers
        for _, p := range ps {
            pairs = append(pairs, &KVPair{Key: p.Key, Value: string(p.Value)})
        }
        d.pairs = pairs
		for _,ch := range d.chans{
			go func(){
				defer func(){
					if r := recover();r != nil{
					}
				}()
				select {
				case ch <- pairs:
				case <- time.After(time.Minute):
					log.Warn("chan is full and new change has been dropped")
				}
			}()
		}
		}
	}
}