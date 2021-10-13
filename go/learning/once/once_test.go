package once_test

import (
	"fmt"
	"testing"
	"sync"
	"unsafe"
)

type Singleton struct{

}

var SingleInstance *Singleton
var once sync.Once

func GetInstance() *Singleton{
	once.Do(func(){
		fmt.Println("Create obj")
		SingleInstance = new(Singleton)
	})
	return SingleInstance
}
func TestOnce(t*testing.T)  {
	var wg sync.WaitGroup
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func() {
			obj := GetInstance()
			fmt.Println(unsafe.Pointer(obj))
			wg.Done()
		}()
	}
	wg.Wait()
}