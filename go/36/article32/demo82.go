package main

import(
	"fmt"
	"strings"
	"io"
	"sync"
	"time"
)

func main(){
	comment := "Package io provides basic interfaces to I/O primitives. " +
		"Its primary job is to wrap existing implementations of such primitives, " +
		"such as those in package os, " +
		"into shared public interfaces that abstract the functionality, " +
		"plus some other related primitives."
	

	//1.
	fmt.Println("New a string reader and name it \"reader1\" ...")
	reader1 := strings.NewReader(comment)
	buf1 := make([]byte,7)
	n,err := reader1.Read(buf1)
	fmt.Printf("buf1:%v,byte(%d)\n",buf1,n)
	fmt.Printf("buf1:%s,byte(%d)\n",buf1,n)
	fmt.Printf("buf1:%q,byte(%d)\n",buf1,n)
	_ = err

	var offset1,index1 int64
	executeIfNoErr(err,func ()  {
		fmt.Printf("Read(%d): %q\n", n, buf1[:n])
		offset1 = int64(53)
		index1 ,err = reader1.Seek(offset1, io.SeekCurrent)
	})

	executeIfNoErr(err, func() {
		fmt.Printf("The new index after seeking from current with offset %d: %d\n",
			offset1, index1)
		n, err = reader1.Read(buf1)
	})

	executeIfNoErr(err, func() {
		fmt.Printf("Read(%d): %q\n", n, buf1[:n])
	})
	fmt.Println()

	//2.
	reader1.Reset(comment)
	num1 := int64(7)
	fmt.Printf("New a limited reader with reader1 and number %d ...\n", num1)
	reader2 := io.LimitReader(reader1,7)//总共七个，没有偏移，一次读完
	buf2 := make([]byte, 2)
	// n, err = reader2.Read(buf2)
	// fmt.Printf("reader2:%s\n",buf2)
	for i := 0; i < 5; i++ {
		n, err = reader2.Read(buf2)
		executeIfNoErr(err, func() {
			fmt.Printf("Read(%d): %q\n", n, buf2[:n])
		})
	}

	// 示例3。
	reader1.Reset(comment)
	offset2 := int64(56)
	num2 := int64(72)
	fmt.Printf("New a section reader with reader1, offset %d and number %d ...\n", offset2, num2)
	reader3 := io.NewSectionReader(reader1, offset2, num2)//总共可以读取72个
	buf3 := make([]byte, 20)
	for i := 0; i < 5; i++ {
		n, err = reader3.Read(buf3)
		executeIfNoErr(err, func() {
			fmt.Printf("Read(%d): %q\n", n, buf3[:n])
		})
	}
	fmt.Println()

	//4
	reader1.Reset(comment)
	writer1 := new(strings.Builder)
	fmt.Println("New a tee reader with reader1 and writer1 ...")
	reader4 := io.TeeReader(reader1,writer1)
	buf4 := make([]byte,10)
	for i := 0; i < 8; i++ {
		n, err = reader4.Read(buf4)//从reader1中读取出来，写到buf4中，作为一个中介
		executeIfNoErr(err, func() {
			fmt.Printf("Read(%d): %q\n", n, buf4[:n])
		})
	}

	// 示例5。
	reader5a := strings.NewReader(
		"MultiReader returns a Reader that's the logical concatenation of " +
			"the provided input readers.")
	reader5b := strings.NewReader("They're read sequentially.")
	reader5c := strings.NewReader("Once all inputs have returned EOF, " +
		"Read will return EOF.")
	reader5d := strings.NewReader("If any of the readers return a non-nil, " +
		"non-EOF error, Read will return that error.")
	
	fmt.Println("New a multi-reader with 4 readers ...")
	reader5 := io.MultiReader(reader5a, reader5b, reader5c, reader5d)
	buf5 := make([]byte, 50)
	for i := 0; i < 8; i++ {
		n, err = reader5.Read(buf5)
		executeIfNoErr(err, func() {
			fmt.Printf("Read(%d): %q\n", n, buf5[:n])
		})
	}//把内容全部发到一个reader中
	fmt.Println()

	// 示例6。
	fmt.Println("New a synchronous in-memory pipe ...")
	pReader,pWriter := io.Pipe()
	_ = interface{}(pReader).(io.ReadCloser)
	_ = interface{}(pWriter).(io.WriteCloser)

	comments := [][]byte{
		[]byte("Pipe creates a synchronous in-memory pipe."),
		[]byte("It can be used to connect code expecting an io.Reader "),
		[]byte("with code expecting an io.Writer."),
	}

	var wg sync.WaitGroup
	wg.Add(2)
	
	go func() {
		defer wg.Done()
		for _, d := range comments {
			time.Sleep(time.Millisecond * 500)
			n,err := pWriter.Write(d)
			if err != nil {
				fmt.Printf("write error: %v\n", err)
				break
			}
			fmt.Printf("Written(%d): %q\n", n, d)
		}
		pWriter.Close()
	}()

	go func() {
		defer wg.Done()
		wBuf := make([]byte, 55)
		for  {
			n, err := pReader.Read(wBuf)
			if err != nil {
				fmt.Printf("read error: %v\n", err)
				break
			}
			fmt.Printf("Read(%d): %q\n", n, wBuf[:n])
		}
	}()
	wg.Wait()
}

func executeIfNoErr(err error, f func ())  {
	if err != nil {
		fmt.Printf("error: %v\n",err)
		return
	}
	f()
}