package main

import(
	"fmt"
)

func swict_test(x interface{})  {
	
	switch i := x.(type) {
	case nil:  
		fmt.Printf(" x 的类型 :%T\n",i)                
	 case int:  
		fmt.Printf("x 是 int 型\n")                      
	 case float64:
		fmt.Printf("x 是 float64 型\n")          
	 case func(int) float64:
		fmt.Printf("x 是 func(int) 型\n")                      
	 case bool, string:
		fmt.Printf("x 是 bool 或 string 型\n" )      
	 default:
		fmt.Printf("未知型")
		
	}
}

func main()  {
	var value float64 = 10
	swict_test(value)
	value2 := 10
	swict_test(value2)
	value3 := "abc"
	swict_test(value3)
	value4 := false
	swict_test(value4)
}