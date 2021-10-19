package obj_pool

import (
	"fmt"
	"runtime"
	"sync"
	"testing"
)

func TestSyncPool(t*testing.T)  {
	pool := sync.Pool{
		New: func () interface{} {
			fmt.Println("Create a new obj.")
			return 100
		},
	}
	v := pool.Get().(int)
	fmt.Println(v)
	pool.Put(3)
	runtime.GC() //GC 会清除sync.pool中缓存的对象
	v1, _ := pool.Get().(int)
	fmt.Println(v1)
}