package main

import "fmt"

func main()  {
	ch1 := make(chan int, 2)

	go func ()  {
		for i := 0; i < 10; i++ {
			fmt.Printf("Sender: sending element %v...\n",i)
			ch1 <- i
		}
		fmt.Printf("Sender:close the channel...")
		close(ch1)
	}()

	for {
		elem ,ok := <- ch1
		if !ok {
			fmt.Println("Recover: closed channel")
			break
		}
		fmt.Printf("Receiver: received an element: %v\n", elem)
	}
	fmt.Println("End.")
}