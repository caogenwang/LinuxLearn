package interface_test

import (
	"fmt"
	"testing"
)

type Animal interface{
	Speak() string
}

type Dog struct {
}

func (d Dog) Speak() string { //Dog实现了Speak，具有类似的继承关系了
	return "Wang Wang!"
}

type Cat struct {
}

func (c *Cat) Speak() string {
	return "Miao Miao!"
}

func TestAnimalFun(t *testing.T) {
	animals := []Animal{Dog{}, new(Cat)}//在接口模式下，可以实现类似于c++中的纯虚函数
	for _, animal := range animals {
		fmt.Println(animal.Speak())
	}
}

func PrintAll(vals []interface{}) {
	for _, val := range vals {
		fmt.Println(val)
	}
}

func TestInterface(t *testing.T) {
	names := []string{"stanley", "david", "oscar"}
	vals := make([]interface{}, len(names))
	for i, v := range names {
		vals[i] = v
	}
	PrintAll(vals)
}
