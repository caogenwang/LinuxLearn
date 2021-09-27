package main

import(
	"fmt"
)

type Test struct{
	Name string
}

var list map[string]Test

func main(){
	list = make(map[string]Test)
	name := Test{"xiaoming"}
	list["name"] = name

	fmt.Println(list["name"].Name)
	list["name"].Name = "Hello"
	fmt.Println(list["name"])
}