// pp
package main

import (
    "fmt"
)

func writeRoutine(test_chan chan int, value int) {
	fmt.Println("write buf start")
    // test_chan <- value
	fmt.Println("write buf stop")
}

func readRoutine(test_chan chan int) {
	fmt.Println("read buf start")
    <-test_chan
	fmt.Println("read buf finish")
    return
}

func main() {

    c := make(chan int)

    x := 100

    // readRoutine(c)
    // go writeRoutine(c, x)

    // writeRoutine(c, x)
    // go readRoutine(c)

    // go readRoutine(c)
    // writeRoutine(c, x)

    go writeRoutine(c, x)
    readRoutine(c)

    fmt.Println(x)
}