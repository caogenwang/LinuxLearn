package main
import "fmt"

type WithName struct{
	Name string
}

type City2 struct{
	WithName
}

type Country2 struct{
	WithName
}

type Printable2 interface{
	PrintStr()
}

func (w *WithName)PrintStr()  {
	fmt.Println(w.Name)
}

// func main()  {
// 	c1 := City2{WithName{"China"}}
// 	c2 := Country2{WithName{"Beijing"}}
// 	c1.PrintStr()
// 	c2.PrintStr()
// }

