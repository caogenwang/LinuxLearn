package main

import "fmt"

type Animal interface {
	ScientificName() string
	Category() string
}

type Name interface {
	Name() string
}

type Pet3 interface {
	Animal
	Name
}

type PetTag struct {
	name  string
	owner string
}

func (pt PetTag) Name() string {
	return pt.name
}

func (pt PetTag) Owner() string {
	return pt.owner
}

type Dog2 struct {
	PetTag
	scientificName string
}

func (dog Dog2) ScientificName() string {
	return dog.scientificName
}

func (dog Dog2) Category() string {
	return "dog"
}

//这里的dog实现了所有的接口方法，Name包含在PetDog

func main() {
	petTag := PetTag{name: "little pig"}
	_, ok := interface{}(petTag).(Name)
	fmt.Printf("PetTag implements interface Named: %v\n", ok)

	dog := Dog2{
		PetTag:         petTag,
		scientificName: "Labrador Retriever",
	}

	_, ok = interface{}(dog).(Animal)
	fmt.Printf("Dog implements interface Animal: %v\n", ok)
	_, ok = interface{}(dog).(Name)
	fmt.Printf("Dog implements interface Named: %v\n", ok)
	_, ok = interface{}(dog).(Pet3)
	fmt.Printf("Dog implements interface Pet3: %v\n", ok) //这里是观察重点

}

// 如果我们把一个值为nil的某个实现类型的变量赋给了接口变量，那么在这个接口变量上仍然可以调用该接口的方法吗？
// 如果可以，有哪些注意事项？如果不可以，原因是什么？

// 这个问题跟一个null指针的c++指针类似，访问静态方法没问题，但是访问属性就出错

// 老师您好，有个地方不理解，对象赋值给接口后，为什么判等操作返回的是true呢？比如上面的例子：
// pet = dog之后紧接着判断pet == dog，返回的是true，按上面的说法，赋值后不是应该被包装成了一个iface吗？这里的判等操作到底是依据什么来判断的呢？麻烦老师解释一下，谢谢~

// 作者回复: 你可以参照Go语言规范中的说明：https://golang.google.cn/ref/spec#Comparison_operators，
// 请注意下面这句：

// A value x of non-interface type X and a value t of interface type T are
// comparable when values of type X are comparable and X implements T.
// They are equal if t's dynamic type is identical to X and t's dynamic value is equal to x.
// 动态类型相同并且动态值相同
