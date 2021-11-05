package main

import(
	"fmt"
	"net"
	"net/http"
	"strings"
	"sync"
	"time"
)

var domains = []string{
	"www.baidu.com",
	"www.docin.com/",
}

func main1()  {
	// 你可以改变myTransport中的各个字段的值，
	// 并观察后面的输出会有什么不同。
	myTransport := &http.Transport{
		Proxy: http.ProxyFromEnvironment,
		DialContext: (&net.Dialer{
			Timeout:       15 * time.Second,
			KeepAlive:     15 * time.Second,
			FallbackDelay: 0,
		}).DialContext,
		MaxConnsPerHost:       2,
		MaxIdleConns:          10,
		MaxIdleConnsPerHost:   2,
		IdleConnTimeout:       30 * time.Second,
		ResponseHeaderTimeout: 0,
		ExpectContinueTimeout: 1 * time.Second,
		TLSHandshakeTimeout:   10 * time.Second,
	}

	myclient := http.Client{
		Transport: myTransport,
		Timeout:   20 * time.Second,
	}

	var wg sync.WaitGroup
	wg.Add(len(domains))

	for _, domain := range domains {
		go func(domain string) {
			var logBuf strings.Builder
			var diff time.Duration
			defer func(){
				logBuf.WriteString(
					fmt.Sprintf("(elapsed time: %s)\n", diff))
				fmt.Println(logBuf.String())
				wg.Done()
			}()
			url := "https://" + domain
			logBuf.WriteString(fmt.Sprintf("Send request to %q with method GET ...\n", url))
			t1 := time.Now()
			resp ,err := myclient.Get(url)
			diff = time.Now().Sub(t1)
			if err != nil {
				logBuf.WriteString(
					fmt.Sprintf("request sending error: %v\n", err))
				return
			}

			defer resp.Body.Close()
			line2 := resp.Proto + " " + resp.Status
			logBuf.WriteString(
				fmt.Sprintf("The first line of response:\n%s\n", line2))


		}(domain)
	}
	wg.Wait()

	resp,_ := http.Get("https://www.docin.com/")
	fmt.Println(resp)
}