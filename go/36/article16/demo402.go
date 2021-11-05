package main

import(
	"fmt"
	"sync"
	"time"
)

var(
	switchFlow chan int
	wg sync.WaitGroup
)

func routine(i int,serialNumber int){
	loop:
	for {
		select{
		case s := <- switchFlow:
			if s == serialNumber{
				fmt.Println(i)
				break loop
			}else{
				switchFlow <- s
			}
		default:
			time.Sleep(10 * time.Millisecond)
		}
	}
	wg.Done()
	switchFlow <- serialNumber+1
}

func main() {
	switchFlow = make(chan int)
	wg.Add(10)
	for i := 0; i < 10; i++ {
		go routine(i, i)
	}
	//引爆点
	switchFlow <- 0

	wg.Wait()
	close(switchFlow)
	fmt.Println("程序结束")

	loop:
	for i:=0;i<10;i++{
		if i == 5 {
			break loop
		}
		fmt.Println(i)
	}
	fmt.Println("Done")
}