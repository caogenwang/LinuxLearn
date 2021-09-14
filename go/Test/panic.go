package main

import (
    "fmt"
)
func call2()  {
	fmt.Println("Enter call2.")
	array := []int{1,2,3,4}
	fmt.Println("%d",array[5])
	fmt.Println("Exit call2.")
}

func call1()  {
	fmt.Println("Enter call1.")
	call2()
	fmt.Println("Exit call1.")
}

func main() {
    fmt.Println("Enter main.")
	call1()
	fmt.Println("Exit main.")
}