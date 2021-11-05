package main

import(
	"fmt"
	"flag"
	"bytes"
	"io"
	"io/ioutil"
	"log"
	"sync"
)

var protecting uint

func init()  {
	flag.UintVar(&protecting,"protecting",1, 
	"It indicates whether to use a mutex to protect data writing.")
}

func main1(){
	flag.Parse()

	const (
		max1 = 5  // 代表启用的goroutine的数量。
		max2 = 10 // 代表每个goroutine需要写入的数据块的数量。
		max3 = 10 // 代表每个数据块中需要有多少个重复的数字。
	)
	
	sign := make(chan struct{}, max1)
	var buffer bytes.Buffer
	var mut sync.Mutex

	for i := 0; i < max1; i++ {//创建多少个go rutine
		go func(id int,writer io.Writer) {
			defer func ()  {
				sign <- struct{}{}
			}()

			for j := 0; j < max2; j++ {
				header := fmt.Sprintf("\n[id: %d,iteration: %d]",id,j)
				data := fmt.Sprintf(" %d",id*j)
				if protecting>0 {
					mut.Lock()
				}
				_,err := writer.Write([]byte(header))
				if err != nil {
					log.Printf("error: %s [%d]",err,id)
				}

				for k := 0; k < max3; k++ {
					_,err := writer.Write([]byte(data))
					if err != nil {
						log.Printf("error: %s [%d]",err,id)
					}	
				}
				if protecting>0 {
					mut.Unlock()
				}
			}
		}(i,&buffer)
	}

	for i := 0; i < max1; i++ {
		<-sign
	}
	
	data, err := ioutil.ReadAll(&buffer)
	if err != nil {
		log.Fatalf("fatal error: %s", err)
	}
	log.Printf("The contents:\n%s", data)
}
