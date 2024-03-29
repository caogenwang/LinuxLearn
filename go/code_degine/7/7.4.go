package main
import (
	"fmt"
	"reflect"
)
func Decorator(decoPtr,fn interface{}) (err error) {
	var decoratedFunc,targetFunc reflect.Value

	decoratedFunc = reflect.ValueOf(decoPtr).Elem() 
	targetFunc = reflect.ValueOf(fn)

	v := reflect.MakeFunc(targetFunc.Type(),func (in []reflect.Value)(out []reflect.Value)  {
		fmt.Println("before") 
		out = targetFunc.Call(in) 
		fmt.Println("after") 
		return
	})
	decoratedFunc.Set(v)    
	return 
}

func foo(a,b,c int) int {
	fmt.Printf("%d, %d, %d \n", a, b, c)
	return a + b + c
}

func bar(a,b string) string {
	fmt.Printf("%s, %s \n", a, b) 
	return a + b
}
type MyFoo func(int, int, int) int

func main()  {
	var myfoo MyFoo
	Decorator(&myfoo, foo)
	x := myfoo(1, 2, 3)
	fmt.Println()
	fmt.Println(x)

	mybar := bar
	Decorator(&mybar, bar)
	v := mybar("hello,", "world!")
	fmt.Println()
	fmt.Println(v)
}