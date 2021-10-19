package main

import(
	"fmt"
	"net/rpc"
)


func main(){
	client,err := rpc.DialHTTP("tcp","localhost:8080")
	if err != nil {
		panic(err.Error())
	}

	var req float32 = 20
	var resp *float32

	err = client.Call("MathUtil.CalculateCircleArea",req,&resp)
	if err != nil {
		panic(err.Error())
	}
	fmt.Println(*resp)
}