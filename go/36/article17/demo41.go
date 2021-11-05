package main

import (
	"fmt"
)

func main() {
	//示例1
	numbers1 := []int{1, 2, 3, 4, 5, 6}
	for i := range numbers1 {
		if i == 3 {
			numbers1[i] |= i
		}
	}
	fmt.Println(numbers1)

	//示例2
	numbers2 := []int{1, 2, 3, 4, 5, 6}
	maxIndex2 := len(numbers2) - 1
	for i, e := range numbers2 {
		if i == maxIndex2 {
			numbers2[0] += e
		} else {
			numbers2[i+1] += e
		}
	}
	fmt.Println(numbers2)

	//示例3
	// value1 := [...]int8{0, 1, 2, 3, 4, 5, 6}
	// switch 1 + 3 { //这条switch语句是无法通过编译的,mismatched types int8 and int
	// case value1[0], value1[1]:
	// 	fmt.Println("0 or 1")
	// case value1[2], value1[3]:
	// 	fmt.Println("2 or 3")
	// case value1[4], value1[5], value1[6]:
	// 	fmt.Println("4 or 5 or 6")
	// }

	value2 := [...]int8{0, 1, 2, 3, 4, 5, 6}
	switch value2[4] {
	case 0, 1:
		fmt.Println("0 or 1")
	case 2, 3:
		fmt.Println("2 or 3")
	case 4, 5, 6:
		fmt.Println("4 or 5 or 6")
	}
}

// 老师，我想问一下，range的副本，是说k、v是副本，还是被迭代的数组是副本？
// 我自己测试在for的里面和外面数组地址是一样的

//作者回复: 迭代变量是副本。另外在Go程序里的变量地址是不能完全说明问题的，因为goroutine的栈空间有可能会被优化。
