package main

import "fmt"

type Cat struct {
	name           string // 名字。
	scientificName string // 学名。
	category       string // 动物学基本分类。
}

func New(name, scientificName, category string) Cat {
	return Cat{
		name:           name,
		scientificName: scientificName,
		category:       category,
	}
}

func (cat *Cat) SetName(name string) {
	cat.name = name
}

func (cat Cat) SetNameOfCopy(name string) {
	cat.name = name
}

func (cat Cat) Name() string {
	return cat.name
}

func (cat Cat) ScientificName() string {
	return cat.scientificName
}

func (cat Cat) Category() string {
	return cat.category
}

func (cat Cat) String() string {
	return fmt.Sprintf("%s (category: %s, name: %q)",
		cat.scientificName, cat.category, cat.name)
}

func main() {
	cat := New("little pig", "American Shorthair", "cat")
	cat.SetName("monster") // (&cat).SetName("monster").自动转化的
	fmt.Printf("The cat: %s\n", cat)

	cat.SetNameOfCopy("little pig")
	fmt.Printf("The cat: %s\n", cat)

	type Pet interface {
		SetName(name string)
		Name() string
		Category() string
		ScientificName() string
	}

	_, ok := interface{}(cat).(Pet)
	fmt.Printf("Cat implements interface Pet: %v\n", ok)
	_, ok = interface{}(&cat).(Pet)
	fmt.Printf("*Cat implements interface Pet: %v\n", ok) //指针类型方法的范围更广
}
//我们可以在结构体类型中嵌入某个类型的指针类型吗？如果可以，有哪些注意事项？
//字面量struct{}代表了什么？又有什么用处？

//思考题1， 我们可以在结构体中嵌入某个类型的指针类型， 它和普通指针类似，默认初始化为nil,因此在用之前需要人为初始化，否则可能引起错误
//思考题2， 空结构体不占用内存空间，但是具有结构体的一切属性，如可以拥有方法，可以写入channel。所以当我们需要使用结构体而又不需要具体属性时可以使用它

//第二次学习本章节，对老师讲解的内容有了清晰的认识。回答一下老师的问题。
//1.一个结构体可以嵌入一个类型的指针类型，而且被嵌入的类型还可以是它自己，这样可以构造出树的数据结构。
//2.struct｛｝是一个空结构体字面量表示，它可以用来给一个变量赋值，这个变量虽然没有字段，但是可以绑定方法。