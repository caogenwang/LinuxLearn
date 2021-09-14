package main

import (
    "fmt"
)

func divide(a, b int) (res int, err error) {
	func() {
		defer func() {
			if rec := recover(); rec != nil {
				err = fmt.Errorf("%s", rec)
			}
		}()
		res = a / b
	}()
	return
}
	
func main() {
	res, err := divide(1, 0)
	// go divide(1,0)
	fmt.Println(res, err) // 0 runtime error: integer divide by zero

	// res, err = divide(2, 1)
	go divide(2,1)
	fmt.Println(res, err) // 2 <nil>
}