package main

import (
	"fmt"
	"time"
)

func getIntChan() <-chan int {
	num := 5
	ch := make(chan int, num)
	for i := 0; i < num; i++ {
		ch <- i
	}
	close(ch)
	return ch
}

func main1() {
	intchan2 := getIntChan()
	for elem := range intchan2 {
		fmt.Printf("The element in intChan2: %v\n", elem)
	}

	intChannels := [3]chan int{
		make(chan int, 1),
		make(chan int, 1),
		make(chan int, 1),
	}

	index := 2
	fmt.Printf("The index: %d\n", index)
	intChannels[index] <- index
	select {
	case <-intChannels[0]:
		fmt.Println("The first candidate case is selected.")
	case <-intChannels[1]:
		fmt.Println("The second candidate case is selected.")
	case elem := <-intChannels[2]:
		fmt.Printf("The third candidate case is selected, the element is %d.\n", elem)
	default:
		fmt.Println("No candidate case is selected!")

	}
	intChan := make(chan int, 1)
	// 一秒后关闭通道。
	time.AfterFunc(time.Second, func() {
		close(intChan)
	})
	select {
	case _, ok := <-intChan:
		if !ok {
			fmt.Println("The candidate case is closed.")
			break
		}
		fmt.Println("The candidate case is selected.")
	}

	// 如果像上述示例那样加入了默认分支，那么无论涉及通道操作的表达式是否有阻塞，select语句都不会被阻塞。如果那几个表达式都阻塞了，或者说都没有满足求值的条件，那么默认分支就会被选中并执行。
	// 如果没有加入默认分支，那么一旦所有的case表达式都没有满足求值条件，那么select语句就会被阻塞。直到至少有一个case表达式满足条件为止。
	// 还记得吗？我们可能会因为通道关闭了，而直接从通道接收到一个其元素类型的零值。所以，在很多时候，我们需要通过接收表达式的第二个结果值来判断通道是否已经关闭。一旦发现某个通道关闭了，我们就应该及时地屏蔽掉对应的分支或者采取其他措施。这对于程序逻辑和程序性能都是有好处的。
	// select语句只能对其中的每一个case表达式各求值一次。所以，如果我们想连续或定时地操作其中的通道的话，就往往需要通过在for语句中嵌入select语句的方式实现。但这时要注意，简单地在select语句的分支中使用break语句，只能结束当前的select语句的执行，而并不会对外层的for语句产生作用。这种错误的用法可能会让这个for语句无休止地运行下去。

}

//1. 这样一条for语句会不断地尝试从intChan2种取出元素值，即使intChan2被关闭，它也会在取出所有剩余的元素值之后再结束执行
//2. 当intChan2中没有元素值时，它会被阻塞在有for关键字的那一行，直到有新的元素值可取
//3. 假设intChan2的值为nil，那么它会被永远阻塞在有for关键字的那一行

//如果在select语句中发现某个通道已关闭，那么应该怎样屏蔽掉它所在的分支?
//当第二个boolean参数为false的时候，在相应的case中设置chan为nil零值，再次case求值的时候会遭遇阻塞，会屏蔽该case

//在select语句与for语句联用时，怎样直接退出外层的for语句？
