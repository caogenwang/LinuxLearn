package main

import (
	"fmt"
	"sync"
)

type Array struct{
	array []int       // 固定大小的数组，用满容量和满大小的切片来代替 24
	len   int		  // 真正长度 8
	cap   int        // 容量 8
	lock  sync.Mutex //为了并发安全使用的锁  8
}

// 新建一个可变长数组
func Make(len, cap int) *Array {
    s := new(Array)
    if len > cap {
        panic("len large than cap")
    }

    // 把切片当数组用
    array := make([]int, cap, cap)

    // 元数据
    s.array = array
    s.cap = cap
    s.len = 0
    return s
}

func (a *Array) Append(element int){
	a.lock.Lock()
	defer a.lock.Unlock()

	if a.len == a.cap{
		newCap := 2 * a.Len
		if a.cap == 0 {
			newCap = 1
		}
		newArray := make([]int,newCap,newCap)
		for k, v := range a.array {
			newArray[k] = v
		}

		a.array = newArray
		a.cap = newCap
	}

	a.array[a.len] = element
	a.len += 1
}

func (a *Array)AppendMany(element...int){
	for _, v := range element {
		a.Append(v)
	}
}

func (a *Array)Get(index int) int {
	if index <= 0 || index >= a.len{
		panic("index over len")
	}
	return a.array[index]
}

func (a *Array)Len() int {
	return a.len
}

func (a *Array)Cap()int{
	return a.cap
}

// 辅助打印
func Print(array *Array) (result string) {
    result = "["
    for i := 0; i < array.Len(); i++ {
        // 第一个元素
        if i == 0 {
            result = fmt.Sprintf("%s%d", result, array.Get(i))
            continue
        }

        result = fmt.Sprintf("%s %d", result, array.Get(i))
    }
    result = result + "]"
    return
}


func main()  {
	// 创建一个容量为3的动态数组
    a := Make(0, 3)
    fmt.Println("cap", a.Cap(), "len", a.Len(), "array:", Print(a))

    // 增加一个元素
    a.Append(10)
    fmt.Println("cap", a.Cap(), "len", a.Len(), "array:", Print(a))

    // 增加一个元素
    a.Append(9)
    fmt.Println("cap", a.Cap(), "len", a.Len(), "array:", Print(a))

    // 增加多个元素
    a.AppendMany(8, 7)
    fmt.Println("cap", a.Cap(), "len", a.Len(), "array:", Print(a))
}

