package main

import "fmt"

func main()  {
	//1
	str := "Go 爱好者 "
	fmt.Printf("The string: %q\n", str)
	fmt.Printf("The len of string: %d\n",len(str))//直接的个数
	fmt.Printf("  => runes(char): %q\n", []rune(str))
	fmt.Printf("  => runes(hex): %x\n", []rune(str))
	fmt.Printf("  => bytes(hex): [% x]\n", []byte(str))

	//2
	for i, c := range str {
		fmt.Printf("%d: %q [% x]\n", i, c, []byte(string(c)))
	}
}