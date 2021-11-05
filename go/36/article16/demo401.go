package main

import(
	"fmt"
	"time"
	"sync"
)

func main1(){
	count := 0
	var mut sync.Mutex

	for i := 0; i < 10; i++ {
		go func(i int) {
			for{
				if i == count{
					fmt.Println(i)
					mut.Lock()
					count++
					mut.Unlock()
				}
				time.Sleep(time.Second)
			}
			
		}(i)
	}
	time.Sleep(time.Second*10)
}