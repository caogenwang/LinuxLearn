package main

import "fmt"

type AnimalCategory struct {
	kingdom string // 界。
	phylum  string // 门。
	class   string // 纲。
	order   string // 目。
	family  string // 科。
	genus   string // 属。
	species string // 种。
}

func (ac AnimalCategory) String() string {
	return fmt.Sprintf("%s%s%s%s%s%s%s",
		ac.kingdom, ac.phylum, ac.class, ac.order,
		ac.family, ac.genus, ac.species)
}

type Animal struct {
	scientificName string
	AnimalCategory //字段声明AnimalCategory代表了Animal类型的一个嵌入字段
}

func (a Animal) Category() string {
	return a.AnimalCategory.String()
}

func main1() {
	category := AnimalCategory{species: "cat"}

	animal := Animal{
		scientificName: "mao",
		AnimalCategory: category,
	}

	fmt.Println(animal.Category())
	fmt.Printf("The animal: %s\n", animal) //嵌入字段AnimalCategory的String方法会被当做animal的方法调用
	//我也为Animal类型编写一个String方法呢？这里会调用哪一个呢
	//答案是，animal的String方法会被调用
	//类型组合也是非侵入式的，它不会破坏类型的封装或加重类型之间的耦合
}

//那么值方法和指针方法之间有什么不同点呢？
//1. 值方法的接收者是该方法所属的那个类型值的一个副本。我们在该方法内对该副本的修改一般都不会体现在原值上，除非这个类型本身是某个引用类型（比如切片或字典）的别名类型
//2.
