package main

import (
	"bufio"
	"bytes"
	"fmt"
	"strings"
)

func main() {
	comment := "Writer implements buffering for an io.Writer object. " +
		"If an error occurs writing to a Writer, " +
		"no more data will be accepted and all subsequent writes, " +
		"and Flush, will return the error. After all data has been written, " +
		"the client should call the Flush method to guarantee all data " +
		"has been forwarded to the underlying io.Writer."
	basicWriter1 := &strings.Builder{} //类似于写入底层

	size := 300
	fmt.Printf("New a buffered writer with size %d ...\n", size)
	//开辟了一个300字节的空间
	writer1 := bufio.NewWriterSize(basicWriter1, size)
	fmt.Println()
	// 示例1。
	begin, end := 0, 53
	fmt.Printf("Write %d bytes into the writer ...\n", end-begin)
	writer1.WriteString(comment[begin:end]) //先写入53个字节的内容
	fmt.Printf("The number of buffered bytes: %d\n", writer1.Buffered())
	fmt.Printf("The number of unused bytes in the buffer: %d\n",
		writer1.Available())
	//尚未flush
	fmt.Printf("The basicWriter1 bytes: %s\n", basicWriter1.String())
	fmt.Println("Flush the buffer in the writer ...")
	writer1.Flush()
	//flush完成
	fmt.Printf("The basicWriter1 bytes: %s\n", basicWriter1.String())
	fmt.Printf("The number of buffered bytes: %d\n", writer1.Buffered())
	fmt.Printf("The number of unused bytes in the buffer: %d\n",
		writer1.Available()) //flush完成之后，缓冲区被清空
	fmt.Println()

	// 示例2。
	begin, end = 0, 326 //一次性写入大于缓冲区的字节数，会直接写入到地层
	fmt.Printf("Write %d bytes into the writer ...\n", end-begin)
	writer1.WriteString(comment[begin:end])
	fmt.Printf("The number of buffered bytes: %d\n", writer1.Buffered())
	fmt.Printf("The number of unused bytes in the buffer: %d\n\n",
		writer1.Available())
	//尚未flush,但是已经超过缓冲区，已经调用了flush
	fmt.Printf("The basicWriter1(%d) bytes: %s\n\n", basicWriter1.Len(), basicWriter1.String())
	fmt.Println("Flush the buffer in the writer ...\n")
	writer1.Flush()
	//flush完成，将剩余的内容flush到
	fmt.Printf("The basicWriter1(%d) bytes: %s\n", basicWriter1.Len(), basicWriter1.String())
	fmt.Println()

	// 示例3。
	basicWriter2 := &bytes.Buffer{}
	fmt.Printf("Reset the writer with a bytes buffer(an implementation of io.ReaderFrom) ...\n")
	writer1.Reset(basicWriter2)
	reader := strings.NewReader(comment)
	fmt.Println("Read data from the reader ...")
	writer1.ReadFrom(reader)
	fmt.Printf("The number of buffered bytes: %d\n", writer1.Buffered())
	fmt.Printf("The number of unused bytes in the buffer: %d\n", writer1.Available()) //unused是还没有被读取
	fmt.Printf("The writer1(%d)\n", writer1.Size())
}
