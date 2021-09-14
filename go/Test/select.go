package main
import (
    "fmt"
    "time"
)
func main() {
    ch := make(chan int)
    go func() {
        for {
            select {
            case <-ch: fmt.Println(0)
            case <-ch: fmt.Println(1)
            }
        }
    }()
    go func() {
        for {
            ch <- 0
        }
    }()
    time.Sleep(2 * 1e9)
}

/*
switch中的default是默认的意思,当所有case不满足的时候,就会执行default
select中的default是当select发现没有case满足,要block时的选择
*/