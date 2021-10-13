package main

import (
	"context"
	"log"

	"github.com/smallnest/rpcx/client"
)

func main() {
	// #1
	d := client.NewPeer2PeerDiscovery("tcp@"+*addr, "")

	// #2
	xclient := client.NewXClient("Arith", client.Failtry, client.RandomSelect, d, client.DefaultOption)
	defer xclient.Close()

	// #3
	args := &example.Args{
		A: 10,
		B: 20,
	}

	// #4
	reply := &example.Reply{}

	// #5
	// err := xclient.Call(context.Backgroud(),"Mul",args,reply)//同步调用

	// if err != nil{
	// 	log.Fatalf("failed to call: %v", err)
	// }

	// log.Printf("%d * %d = %d", args.A, args.B, reply.C)

	call, err := xclient.Go(context.Background(), "Mul", args, reply, nil) //异步调用
	if err != nil {
		log.Fatalf("failed to call: %v", err)
	}

	replyCall := <-call.Done
	if replyCall.Error != nil {
		log.Fatalf("failed to call: %v", replyCall.Error)
	} else {
		log.Printf("%d * %d = %d", args.A, args.B, reply.C)
	}
}
