package main

import(
	"fmt"
	"runtime"
	"sync"
	"time"
)

func main() {
	myPool := &sync.Pool{
		New: func() interface{} {
			fmt.Println("Create new obj")
			return 0
		},
	}
 
	myPool.Put(1)
	myPool.Put(2)
 
	time.Sleep(time.Second * 2)

 	runtime.GC() //GC 会清除sync.pool中缓存的对象

	p1 := myPool.Get().(int)
	fmt.Println(p1) // 2
 
	p2 := myPool.Get().(int)
	fmt.Println(p2) // 0
}