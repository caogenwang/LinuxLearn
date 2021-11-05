package main

import(
	"fmt"
)

func main2()  {
	for i := 0; i < 10; i++ {
		go func ()  {
			fmt.Println(i)
		}(i)
	}
}