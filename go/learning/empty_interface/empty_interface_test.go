package empty_interface

import(
"fmt"
"testing"
)

func Dosomething(p interface{}){
	if i,ok := p.(int);ok{
		fmt.Println("integer ",i)
		return
	}
	if i,ok := p.(string);ok{
		fmt.Println("string ",i)
		return
	}
	fmt.Println("Unknow Type")
}

func TestDosomething(t*testing.T)  {
	Dosomething(10)
	Dosomething("10")
}