package concurrency

import (
	"fmt"
	"runtime"
	"testing"
	"time"
)

func runTask(id int) string {
	time.Sleep(10 * time.Millisecond)
	return fmt.Sprintf("The result is from %d", id)
}

func FirstResponse() string{
	ch := make(chan string,10)
	for i := 0; i < 10; i++ {
		go func (i int)  {
			ret := runTask(i)
			ch <- ret
		}(i)
	}
	return <-ch
}

func TestFirstResponse(t *testing.T) {
	t.Log("Before:", runtime.NumGoroutine())
	t.Log(FirstResponse())
	time.Sleep(time.Second * 1)
	t.Log("After:", runtime.NumGoroutine())

}