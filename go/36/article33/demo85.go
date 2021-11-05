package main

import (
	"bufio"
	"fmt"
	"strings"
)

func main2() {
	comment := "Package bufio implements buffered I/O. " +
		"It wraps an io.Reader or io.Writer object, " +
		"creating another object (Reader or Writer) that " +
		"also implements the interface but provides buffering and " +
		"some help for textual I/O."
	basicReader := strings.NewReader(comment)
	fmt.Printf("The size of basic reader: %d\n", basicReader.Size())
	size := 300
	fmt.Printf("New a buffered reader with size %d ...\n", size)
	reader1 := bufio.NewReaderSize(basicReader, size) //只开辟了300个空间
	_ = reader1
	fmt.Println()
	fmt.Print("[ About 'Peek' method ]\n\n")

	//1
	peekNum := 38
	fmt.Printf("Peek %d bytes ...\n", peekNum)
	bytes, err := reader1.Peek(peekNum)
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("Peeked contents(%d): %q\n", len(bytes), bytes)
	fmt.Printf("The number of unread bytes in the buffer: %d\n", reader1.Buffered())
	fmt.Println()

	fmt.Print("[ About 'Read' method ]\n\n")

	// 示例2。
	readNum := 38
	buf1 := make([]byte, readNum)
	fmt.Printf("Read %d bytes ...\n", readNum)
	n, err := reader1.Read(buf1)
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("Read contents(%d): %q\n", n, buf1)
	fmt.Printf("The number of unread bytes in the buffer: %d\n", reader1.Buffered())
	fmt.Println()

	fmt.Print("[ About 'ReadSlice' method ]\n\n")
	// 示例3。
	fmt.Println("Reset the basic reader ...")
	basicReader.Reset(comment)
	fmt.Println("Reset the buffered reader ...")
	reader1.Reset(basicReader)
	fmt.Println()

	delimiter := byte('(')
	fmt.Printf("Read slice with delimiter %q...\n", delimiter)
	line, err := reader1.ReadSlice(delimiter) //读到分隔符就结束
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("Read contents(%d): %q\n", len(line), line)
	fmt.Printf("The number of unread bytes in the buffer: %d\n", reader1.Buffered())
	fmt.Println()

	delimiter = byte('[')
	fmt.Printf("Read slice with delimiter %q...\n", delimiter)
	line, err = reader1.ReadSlice(delimiter)
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("Read contents(%d): %q\n", len(line), line)
	fmt.Printf("The number of unread bytes in the buffer: %d\n", reader1.Buffered())
	fmt.Println()

	// 示例4。
	fmt.Println("Reset the basic reader ...")
	basicReader.Reset(comment)
	size = 200
	fmt.Printf("New a buffered reader with size %d ...\n", size)
	reader2 := bufio.NewReaderSize(basicReader, size)
	fmt.Println()

	delimiter = byte('[')
	fmt.Printf("Read slice with delimiter %q...\n", delimiter)
	line, err = reader2.ReadSlice(delimiter) //找不到分隔符，一次性读完，只读了200个
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("Read contents(%d): %q\n", len(line), line)
	fmt.Printf("The number of unread bytes in the buffer: %d\n", reader2.Buffered())
	fmt.Println()

	fmt.Print("[ About 'ReadBytes' method ]\n\n")

	// 示例5。
	fmt.Println("Reset the basic reader ...")
	basicReader.Reset(comment)
	size = 200
	fmt.Printf("New a buffered reader with size %d ...\n", size)
	reader3 := bufio.NewReaderSize(basicReader, size)
	fmt.Println()

	delimiter = byte('[')
	fmt.Printf("Read bytes with delimiter %q...\n", delimiter)
	line, err = reader3.ReadBytes(delimiter) //全部读取完整
	if err != nil {
		fmt.Printf("error: %v\n", err)
	}
	fmt.Printf("Read contents(%d): %q\n", len(line), line)
	fmt.Printf("The number of unread bytes in the buffer: %d\n", reader3.Buffered())
	fmt.Println()
	//如果在填满缓冲区之后仍然未能找到分隔符，那么该方法就会把整个
	//缓冲区作为第一个结果值返回，同时返回缓冲区已满的错误。

	//ReadBytes方法会通过调用ReadSlice方法，一次又一次地填充缓冲区，
	//并在其中寻找分隔符。除非发生了未预料到的错误或者找到了分隔符，否则这一过程将会一直进行下去。

	// 示例6和示例7。
	fmt.Print("[ About contents leak ]\n\n")
	showContentsLeak(comment)
}
