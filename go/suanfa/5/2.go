package main

import (
	"fmt"
	"reflect"
)

type A interface{
	Println()
}

// 定义一个接口，有两个方法
type B interface {
    Println()
    Printf() int
}

type A1instance struct{
	Data string
}

func(a* A1instance)Println(){ //实现了接口A
	fmt.Println(a.Data)
}

type B1instace struct {
	Data string
}

func (b *B1instace)Println()  {//B1instace既是A的接口又是B的接口实现
	fmt.Println(b.Data)
}

func (b*B1instace)Printf() int {
	fmt.Printf("%s\n",b.Data)
	return 0
}

func main(){

	var a A
	// 将具体的结构体赋予该变量
	a = &A1instance{"I love china!"}
	a.Println()
	fmt.Printf("%s\n",reflect.TypeOf(a))

	// 将具体的结构体赋予该变量
	a = &B1instace{"I love china too"}
	a.Println()
	// a.Printf()//A接口中没有实现Printf，所以是错的

	var  b B
	// b = &A1instance{Data: "i love beijing"}//A1instance只是A的接口实现，不是B的接口实现
	//接口b指向具体的类型
	b = &B1instace{Data: "I love beijing"}
	b.Println()
	b.Printf()
	fmt.Printf("%s\n",reflect.TypeOf(b))
}
