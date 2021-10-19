package uintTest

import (
	"fmt"
	"testing"
)

func TestFunctions(t*testing.T)  {
	input := [...] int{1,2,3}
	output := [...] int{1,4,9}
	for i := 0; i < len(input); i++ {
		res := Squre(input[i])
		fmt.Println(res)
		if output[i] == res {
			fmt.Println("Canclate correcnt")
		}
	}
}