package main

import(
	"fmt"
	"errors"
)

func echo(reqeset string) (response string,err error){
	if reqeset == "" {
		err = errors.New("empty request")
		return
	}
	response = fmt.Sprintf("echo:hello %s\n",reqeset)
	return
}

func main2()  {
	for _, req := range []string{"","xiaowang"} {
		resp,err := echo(req)
		if err != nil {
			fmt.Printf("error: %s\n", err)
			continue
		}
		fmt.Printf("resp:%s",resp)
	}
}