package main

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"log"
	"sync"
	"time"
)

type singleHandler func() (data string,n int,err error)

type handlerConfig struct{
	handler   singleHandler // 单次处理函数。
	goNum     int           // 需要启用的goroutine的数量。
	number    int           // 单个goroutine中的处理次数。
	interval  time.Duration // 单个goroutine中的处理间隔时间。
	counter   int           // 数据量计数器，以字节为单位。
	counterMu sync.Mutex    // 数据量计数器专用的互斥锁。
}

func (hc *handlerConfig)count(increment int) int {
	hc.counterMu.Lock()
	defer hc.counterMu.Unlock()
	hc.counter += increment
	return hc.counter
}

func main()  {
	var mut sync.Mutex

	genWriter := func (writer io.Writer) singleHandler {
		return func () (data string,n int,err error)  {
			data = fmt.Sprintf("%s\t",time.Now().Format(time.StampNano))

			mut.Lock()
			defer mut.Unlock()
			n,err = writer.Write([]byte(data))
			return
		}
	}

	genReader := func(reader io.Reader) singleHandler {
		return func()(data string,n int,err error){
			buffer,ok := reader.(*bytes.Buffer)
			if !ok {
				err = errors.New("unsupported reader")
				return
			}

			mut.Lock()
			defer mut.Unlock()
			data,err = buffer.ReadString('\t')
			n = len(data)
			return
		}
	}

	// buffer 代表缓冲区。
	var buffer bytes.Buffer

	writingConfig := handlerConfig{
		handler:  genWriter(&buffer),
		goNum:    5,
		number:   4,
		interval: time.Millisecond * 100,
	}

	readingConfig := handlerConfig{
		handler:  genReader(&buffer),
		goNum:    10,
		number:   2,
		interval: time.Millisecond * 100,
	}

	sign := make(chan struct{}, writingConfig.goNum+readingConfig.goNum)

	for i := 0; i < writingConfig.goNum; i++ {
		go func(i int) {
			defer func ()  {
				sign <- struct{}{}
			}()

			for j := 0; j <= writingConfig.number; j++ {
				time.Sleep(writingConfig.interval)
				data,n,err := writingConfig.handler()
				if err != nil {
					log.Printf("writer [%d-%d]: error: %s",i, j, err)
					continue
				}
				total := writingConfig.count(n)
				log.Printf("writer [%d-%d]: %s (total: %d)",i, j, data, total)
			}

		}(i)
	}
}



