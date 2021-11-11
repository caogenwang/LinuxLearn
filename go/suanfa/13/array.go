package main

import "fmt"

func main(){

	array := make([]int,0,2)//make(type,len,cap)
	fmt.Println("cap", cap(array), "len", len(array), "array:", array)
	fmt.Println()

	// 虽然 append 但是没有赋予原来的变量 array,相当于复制了一个array
	_ = append(array, 1)
	fmt.Println("cap", cap(array), "len", len(array), "array:", array)
	_ = append(array, 1)
	fmt.Println("cap", cap(array), "len", len(array), "array:", array)
	_ = append(array, 1)
	fmt.Println("cap", cap(array), "len", len(array), "array:", array)

	fmt.Println("-------")
	// 赋予回原来的变量
    array = append(array, 1)
    fmt.Println("cap", cap(array), "len", len(array), "array:", array)
    array = append(array, 1)
    fmt.Println("cap", cap(array), "len", len(array), "array:", array)
    array = append(array, 1)
    fmt.Println("cap", cap(array), "len", len(array), "array:", array)
    array = append(array, 1, 1, 1, 1)
    fmt.Println("cap", cap(array), "len", len(array), "array:", array)
    array = append(array, 1, 1, 1, 1, 1, 1, 1, 1, 1)
    fmt.Println("cap", cap(array), "len", len(array), "array:", array)

}