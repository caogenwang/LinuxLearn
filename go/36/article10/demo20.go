package main

import "fmt"


func main1()  {
	ch1 := make(chan int,3)

	ch1 <- 2
	ch1 <- 1
	ch1 <- 3

	elem1 := <-ch1

	fmt.Printf("The first element received from channel ch1: %v\n",elem1)

	elem1 = <-ch1

	fmt.Printf("The second element received from channel ch1: %v\n",elem1)

	elem1 = <-ch1

	fmt.Printf("The third element received from channel ch1: %v\n",elem1)
}