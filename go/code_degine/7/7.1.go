package main


import "fmt"

func decorator(f func(s string)) func(s string){
	return func(s string){
		fmt.Println("started")
		f(s)
		fmt.Println("Done")
	}
}

func Hello(s string)  {
	fmt.Println("Hello,",s)
}

// func main()  {
// 	decorator(Hello)("go!")

// 	hello := decorator(Hello)
// 	hello("go again!")
// }