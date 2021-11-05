package main

import "fmt"

func main2() {
	defer fmt.Println("first defer")
	for i := 0; i < 3; i++ {
		defer fmt.Printf("defer in for [%d]\n", i)
	}
	defer fmt.Println("last defer")
}
// last defer
// defer in for [2]
// defer in for [1]
// defer in for [0]
// first defer