package main

import "fmt"

var c = make(chan int, 1)//有缓冲

func f() {

    c <- 'c'

    fmt.Println("在goroutine内")
}

func main() {
    go f()

    c <- 'c'
    <-c
    <-c

    fmt.Println("外部调用")
}