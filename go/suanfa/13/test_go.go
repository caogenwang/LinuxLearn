package main

import "fmt"

type pArr []int

func (p *pArr) oopFunc() {
    (*p)[3] = 111
}

type newFunc func(string)

func strFunc(s string) {
    fmt.Printf("%T ", s[1]) //uint8
    fmt.Println(s)          //hello world
}

type Person struct {
    Name   string
    Height int
}

func changeStruct(p Person) {
    p.Name = "李四"
    p.Height = 160
}

func main() {

    //切片指针的使用
    var ptr []*int
    i := 1
    ptr = append(ptr, &i)
    fmt.Println("ptr:", ptr)

    //结构体是值类型
    person := Person{
        "张三",
        180,
    }
    changeStruct(person)
    fmt.Println(person) //{张三 180}

    //func 可以作为参数进行传递
    var nf newFunc
    nf = strFunc
    str := "hello world"
    nf(str) //uint8 hello world

    //类似面向对象的方法
    p := make(pArr, 4)
    p.oopFunc()
    fmt.Println("p:", p) //p: [0 0 0 111]

    //值类型无法被改变
    num := 1
    valPass(num)
    fmt.Println("num:", num) //num: 1

    //引用类型可以在函数中被改变
    nums := []int{0, 1, 2, 3}
    RefPass(nums)
    fmt.Println("nums:", nums) //nums: [0 100 2 3]

    //形参可以改变引用类型的值，但不能够形参重新赋值
    RefPass2(nums)
    fmt.Println("nums:", nums) //nums: [0 100 2 3]

    //形参可以改变指针类型的值
    n := new(int)
    *n = 111
    PointPass(n)
    fmt.Println("n:", *n) //n: 12

    //形参可以改变指针类型的值,但是对形参重新赋值，不会影响实参
    PointPass2(n)
    fmt.Println("n:", *n) //n: 12

}

//指针传递，对指针重新赋值，指针指向了新的地址，此时对形参做修改将不再影响外面的实参
func PointPass2(num *int) {
    num = new(int)//改变的是
    *num = 13
}

// 指针传递，普通用法
func PointPass(num *int) {
    *num = 12
}

//引用传递，普通用法，这个会改变外面的实参
func RefPass(nums []int) {
    nums[1] = 100
}

//引用传递，对形参重新赋值，不会改变外面的实参，形参指向了新的地址
func RefPass2(nums []int) {
    nums = []int{9, 8, 7, 6}
}

//值传递
func valPass(num int) {
    num = 5
}