package main

import(
	"errors"
	"fmt"
)

func main()  {
	fmt.Println("Enter main function")

	defer func(){
		fmt.Println("Enter defer function")
		if p := recover();p != nil {
			fmt.Printf("panic: %s\n",p)
			panic(errors.New("something wrong2"))
		}
		fmt.Printf("Exit defer function.\n")
	}()

	// recover函数的错误用法。
	fmt.Printf("no panic: %v\n", recover())

	// 引发panic。
	panic(errors.New("something wrong"))

	// recover函数的错误用法。
	p := recover()
	fmt.Printf("panic: %s\n", p)

	fmt.Println("Exit function main.")
}