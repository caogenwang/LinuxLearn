package main

import(
	"fmt"
	"math/rand"
	"time"
)

func queryFromSrc(src string)(ret string) {
	nanoSec := time.Now().Nanosecond()
    rand.Seed(int64(nanoSec))
    sec := (rand.Int31() % 10) + 1
    // time sleep simulates dns lookup and query
    time.Sleep(time.Second * time.Duration(sec))
    ret = fmt.Sprintf("src=%s use sec=%d", src, sec)
    fmt.Println("a query ok, ret=", ret)
    return ret
}

func multiQuery() (ret string) {
	// res := make(chan string, 3)
	res := make(chan string)
	go func() {
		res <- queryFromSrc("ns1.dnsserver.com")
	}()
	go func() {
        res <- queryFromSrc("ns2.dnsserver.com")
    }()
    go func() {
        res <- queryFromSrc("ns3.dnsserver.com")
    }()
	return <-res
}

func main()  {
	fmt.Println("start multi query:\n")
	res := multiQuery()
	fmt.Println("res=",res)
	// time.Sleep(time.Second * 20)
}