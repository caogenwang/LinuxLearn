package main

import (
    "fmt"
)

func main() {
    num := 65
    str := string(num)
    fmt.Printf("%v, %T\n", str, str)
}