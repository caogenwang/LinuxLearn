package main

import (
	"bytes"
	"fmt"
)

func main2() {
	var buffer1 bytes.Buffer
	contents := "Simple byte buffer for marshaling data."
	fmt.Printf("Writing contents %q ...\n", contents)
	buffer1.WriteString(contents)
	fmt.Printf("The length of buffer: %d\n", buffer1.Len())   //39
	fmt.Printf("The capacity of buffer: %d\n", buffer1.Cap()) //64

	p1 := make([]byte, 7)
	n, _ := buffer1.Read(p1)
	fmt.Printf("%d bytes were read.(call Read)\n", n)         //7
	fmt.Printf("The length of buffer: %d\n", buffer1.Len())   //32 = 39 - 7
	//bytes.Buffer的Len方法返回的是内容容器中未读部分的长度，而不是其中已存内容的总长度
	fmt.Printf("The capacity of buffer: %d\n", buffer1.Cap()) //64
	//1.读取内容时，相应方法会依据已读计数找到未读部分，并在读取后更新计数。
	//2.写入内容时，如需扩容，相应方法会根据已读计数实现扩容策略。
	//3.截断内容时，相应方法截掉的是已读计数代表索引之后的未读部分。
	//4.读回退时，相应方法需要用已读计数记录回退点
	//5.重置内容时，相应方法会把已读计数置为0。
	//6.导出内容时，相应方法只会导出已读计数代表的索引之后的未读部分。
	//7.获取长度时，相应方法会依据已读计数和内容容器的长度，计算未读部分的长度并返回。

}
