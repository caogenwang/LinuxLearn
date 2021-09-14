package main

import (
"fmt"
"time"
)
func getChars(s string)  {
    for _,c := range s{
        time.Sleep(time.Millisecond * 2)
        fmt.Printf("%c ",c)
    }
}

func getDigits(s []int)  {
    for _, d := range s {
        time.Sleep(time.Millisecond * 4)
        fmt.Printf("%d ",d)
    }
}

func main(){
    fmt.Println("main execution started\n")

    go getChars("Hello")

    go getDigits([]int{1,2,3,4,5})

    time.Sleep(time.Millisecond*200)

    fmt.Println("\nmain execution stop")
}

