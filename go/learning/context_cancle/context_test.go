package context_cancle

import(
	"context"
	"fmt"
	"testing"
	"time"
)

func isCancelled(ctx context.Context)bool {
	select {
	case <- ctx.Done()://管道哟返回
		return true
	default:
		return false
	}
}

func TestContextCancle(t*testing.T)  {
	ctx ,cancel := context.WithCancel(context.Background())//cancel是个函数
	for i := 0; i < 5; i++{
		go func (i int, ctx context.Context)  {
			for{
				if isCancelled(ctx) {
					break
				}
				time.Sleep(time.Millisecond * 50)
			}
			fmt.Println(i, "Cancelled")
		}(i,ctx)
	}
	time.Sleep(time.Second * 1)
	cancel()
}