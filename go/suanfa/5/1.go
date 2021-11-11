package main

import (
	"fmt"
	"reflect"
)

func Print(a interface{}){
	fmt.Printf("%T,%v\n", a, a)
}

func main(){
	var a interface{}
	a = 2
	fmt.Printf("%T,%v\n", a, a)

	Print(a)
	Print(3.0)
	Print("I love china!")

	//使用断言
	v,ok := a.(int)
	if ok {
		Print(v)
	}
	a = "i am joker"
	switch a.(type) {
	case int:
		fmt.Println("a is int")
	case float64:
		fmt.Println("a is float64")
	case string:
		fmt.Println("a is string")
	default:
		fmt.Println("a not type found type")
	}

	t := reflect.TypeOf(a)
	fmt.Printf("a is type: %s\n", t.Name())
}