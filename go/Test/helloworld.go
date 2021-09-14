package main

import (
"fmt"
"time"
)
 func printf()  {
    time.Sleep(time.Second*3)
     fmt.Println("Hello world.")
 }

func main(){
    fmt.Println("Hello World!")
    go printf() 
    time.Sleep(time.Second*2)
    fmt.Println("Hello World!")
}