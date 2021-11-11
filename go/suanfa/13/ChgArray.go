package main

import (
	"fmt"
	"sync"
	"unsafe"
)

// 在go语言中，make和new都是内存的分配（堆上），但是make只用于slice、map以及channel的初始化（非零值）；而new用于类型的内存分配，并且内存置为零。make返回的是引用类型本身；而new返回的是指向类型的指针。
//小结：
// make和new都是golang用来分配内存的內建函数，且在堆上分配内存，make 即分配内存，也初始化内存。new只是将内存清零，并没有初始化内存。
// make返回的还是引用类型本身；而new返回的是指向类型的指针。
// make只能用来分配及初始化类型为slice，map，channel的数据；new可以分配任意类型的数据。
type Array struct{
	array []int       // 固定大小的数组，用满容量和满大小的切片来代替 24
	len   int		  // 真正长度 8
	cap   int        // 容量 8
	lock  sync.Mutex //为了并发安全使用的锁  8
}
type Interface interface{//interface本身是一个引用类型
	Len() int
	Cap() int
}
// func Make(len,cap int) *Array{
// 	s := new(Array)
// 	fmt.Println(sizeofz(s))
// }

// func (ar Array)Len()int{
// 	return ar.len
// }

// func (ar Array)Cap()int{
// 	return ar.cap
// }

func (ar Array)Len()int{
	return ar.len
}

func (ar Array)Cap()int{
	return ar.cap
}

func main(){
	var s *Array
	s = new(Array)
	fmt.Println(unsafe.Sizeof(s))//s是指针类型，为8个字节,

	s1 := Array{}
	s = &s1
	fmt.Println(unsafe.Sizeof(s1))//48

	// in := Interface{} 接口不能被实例化
	// fmt.Println(unsafe.Sizeof(in))

	var arin Interface
	arin = &s1//之所以能这样赋值，需要Array的方法接收者都是非指针类型
	_ = arin

	// var arin2 *Interface
	// arin2 = &s1
	// _ = arin2
	//err *Interface is pointer to interface, not interface

}
//如果都是指针接收者，接口类型指向对象的指针类型，不能指向对象类型
//如果是对象接收者，接口可以指向指针类型或者对象类型
//对象接收者和指针接收者的区别是能否改变对象内的值

// go语言中数组和切片的区别

// ●　切片是指针类型，数组是值类型

// ●　数组的长度是固定的，而切片不是（切片是动态的数组）

// ●　切片比数组多一个属性：容量（cap)

// ●　切片的底层是数组