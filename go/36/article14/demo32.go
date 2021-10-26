package main

import "fmt"

type Pet interface {
	Name() string
	Category() string
	SetName(name string)
}

type Dog struct {
	name string
}

func (dog *Dog) SetName(name string) {
	dog.name = name
}

func (dog Dog) Name() string {
	return dog.name
}

func (dog Dog) Category() string {
	return "dog"
}

func main() {
	//1
	dog := Dog{"little pig"}
	fmt.Printf("The dog's name is %q.\n", dog.Name())
	var pet Pet = &dog
	dog.SetName("monster")
	fmt.Printf("The dog's name is %q.\n", dog.Name())
	fmt.Printf("This pet is a %s, the name is %q.\n",
		pet.Category(), pet.Name())
	fmt.Println()


	//当我们给一个接口变量赋值的时候，该变量的动态类型会与它的动态值一起被存储在一个专用的数据结构中
	// iface的实例会包含两个指针，一个是指向类型信息的指针，另一个是指向动态值的指针
	// 总之，接口变量被赋予动态值的时候，存储的是包含了这个动态值的副本的一个结构更加复杂的值

	var dog1 *Dog
	fmt.Println("The first dog is nil. [wrap1]")
	dog2 := dog1
	fmt.Println("The second dog is nil. [wrap1]")
	var pet2 Pet = dog2
	if pet2 == nil {
		fmt.Println("The pet is nil. [wrap1]")
	} else {
		fmt.Println("The pet is not nil. [wrap1]")
	}
	fmt.Printf("the type of pet2 is %T\n", pet2)

	//当我们把dog2的值赋给变量pet的时候，dog2的值会先被复制，不过由于在这里它的值是nil，所以就没必要复制了。
	// 然后，Go 语言会用我上面提到的那个专用数据结构iface的实例包装这个dog2的值的副本，这里是nil。
	// 虽然被包装的动态值是nil，但是pet的值却不会是nil，因为这个动态值只是pet值的一部分而已
	// 顺便说一句，这时的pet的动态类型就存在了，是*Dog。我们可以通过fmt.Printf函数和占位符%T来验证这一点，另外reflect包的TypeOf函数也可以起到类似的作用
	//当我们把这个变量赋给pet的时候，Go 语言会把它的类型和值放在一起考虑
}
