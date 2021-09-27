package main

import (
	"fmt"
	// "time"
)

//执行顺序，res如果出现错误，会报panic错误，然后执行defer函数，其中recover捕获错误
func divide(a, b int) (res int, err error) {
	go func() {
		// defer func() {
		// 	fmt.Println("cuole")
		// 	if rec := recover(); rec != nil {
		// 		err = fmt.Errorf("%s", rec)
		// 	}
		// }()
		res = a / b
		// panic("cuole")
	}()
	return
}

func main() {
	// go divide(1,0)
	// defer func ()  {
	// 	if rec := recover(); rec != nil {
	// 		fmt.Errorf("%s", rec)
	// 	}
	// }()
	res, err := divide(1, 0)
	// go divide(1,0)
	fmt.Println(res, err) // 0 runtime error: integer divide by zero

	// res, err = divide(2, 1)
	// go divide(2, 1)
	// fmt.Println(res, err) // 2 <nil>
	// time.Sleep(time.Second)

}
