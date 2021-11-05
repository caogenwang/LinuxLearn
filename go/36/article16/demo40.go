package main

import (
	"fmt"
	"sync/atomic"
	"time"
)

func main1()  {
	var count uint32 = 5
	trigger := func (i uint32,fn func())  {
		for{
			if n := atomic.LoadUint32(&count); n == i{
				fn()
				atomic.AddUint32(&count,1)
				break
			}
			time.Sleep(time.Nanosecond)
		}
	}

	for i := uint32(0); i < 10; i++ {
		go func(i uint32){
			fn := func ()  {
				fmt.Println(i)
			}
			trigger(i,fn)
		}(i)
	}
	trigger(10, func() {})
}
//atomic的加操作和读操作只有32位和64位整数型，所以必须要把int转为intxx。
//之所以这么做是因为int位数是根据系统决定的，而原子级操作要求速度尽可能的快，所以明确了整数的位数才能最大地提高性能。
//Go语言调度goroutine是准抢占式的，虽然会防止某个goroutine运行太久，
//并做换下处理。但是像简单的死循环这种有可能会换下失败，尤其是windows下，这跟操作系统的底层支持有关。不过一般情况下不用担心。