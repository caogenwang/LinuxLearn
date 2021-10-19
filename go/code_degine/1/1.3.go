package main

import "fmt"

type City3 struct{
	Name string
}
type Country3 struct{
	Name string
}

func (c *City3)ToString() string {
	return "City3 = " + c.Name
}

func (c *Country3)ToString() string {
	return "Country = " + c.Name
}

type Stringable interface {
	 ToString() string
}

func PrintStr(p Stringable) { 
	fmt.Println(p.ToString())
}

func main()  {
	d1 := Country3 {"USA"}
	d2 := City3{"Los Angeles"}
	PrintStr(&d1)
	PrintStr(&d2)
}