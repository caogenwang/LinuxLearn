package main

import(
	"fmt"
)

type Container []interface{}

func (c*Container) Put(elem interface{}){
	*c = append(*c,elem)
}

func (c*Container) Get()interface{}{
	elem := (*c)[0]
	*c = (*c)[1:]
	return elem
}

// func main()  {
// 	intContainer := &Container{}
// 	intContainer.Put(7)
// 	intContainer.Put(42.0)

// 	x,ok := intContainer.Get().(int)
// 	if !ok {
// 		fmt.Println("Unable to read an int from intContainer")
// 	}
// 	y,ok := intContainer.Get().(int)
// 	if !ok {
// 		fmt.Println("Unable to read an int from intContainer")
// 	}
// 	fmt.Println(x,y)
// }