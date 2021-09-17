package main

import(
	"fmt"
	"os"
	"encoding/json"
)

func main()  {
	dec := json.NewDecoder(os.Stdin)
	enc := json.NewEncoder(os.Stdout)

	for{
		var v map[string]interface{}
		if err := dec.Decode(&v);err != nil{
			fmt.Println(err)
			return
		}

	}

}

