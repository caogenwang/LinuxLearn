package main

import(
	"math"
	"net/rpc"
	"net"
	"net/http"
)

type MathUtil struct{

}

func (mu* MathUtil)CalculateCircleArea(req float32,resp*float32)error  {
	*resp = math.Pi * req * req
	return nil
}

func main()  {
	mathutil := new(MathUtil)
	err := rpc.Register(mathutil)
	if err !=  nil{
		panic(err.Error())
	}
	rpc.HandleHTTP()

	listen,err := net.Listen("tcp",":8080")
	if err !=  nil{
		panic(err.Error())
	}
	http.Serve(listen,nil)
}
