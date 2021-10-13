package main

import (
    "context"
    "fmt"
    "time"
)

func main() {
    gen := func(ctx context.Context) <-chan int {
        dst := make(chan int)
        n := 1
        go func() {
            for {
                select {
                case <-ctx.Done():
                    fmt.Println("************\n")
                    return // returning not to leak the goroutine
                case dst <- n://n的值一直往管道里写
                    n++
                }
            }
        }()
        return dst
    }

    ctx, cancel := context.WithCancel(context.Background())
    defer cancel() // cancel when we are finished consuming integers
    time.Sleep(time.Millisecond * 100)
    for n := range gen(ctx) {
        fmt.Println(n)
        if n == 5 {
            break
        }
    }
}