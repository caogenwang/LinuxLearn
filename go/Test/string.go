package main

import "fmt"
type ConfigOne struct {
	Daemon string
	Number int
}

// func (c *ConfigOne) String() string {
// 	return fmt.Sprintf("print: %v", c)
// }

func main()  {
	c := &ConfigOne{}
	c.Daemon = "123"
	c.Number = 5
	// _ = c.String()
	fmt.Printf("print: %v", *c)
	fmt.Println()
}