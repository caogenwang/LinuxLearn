package main

import(
	"fmt"
	"time"
)

var c = make(chan int)

func goRoutine(){
	fmt.Println("go routine start")
	time.Sleep(time.Second*3)
	c <- 2
}

func main()  {
	go goRoutine()
	fmt.Println(<-c)
	fmt.Println("finish.")
}