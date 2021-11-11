package main

import "sync.Mutex"


type ArrayStack struct{
	array []string
	size  int
	lock  sync.Mutex
}

func (a *ArrayStack)Push(value int)  {
	a.lock.Lock()
	defer a.lock.Unlock()
	var newCap int 
	if a.size == Cap(a.array) {
		newCap = 2 * a.size
		if Cap(a.array) == 0 {
			newCap = 1
		}

		array := make([]string, a.size+1,newCap)
		for index, v := range a.array {
			array[index] = v
		}
		a.array = array
	}

	a.array[a.size] = value
	a.size += 1
}

func (a *ArrayStack)Pop() string {
	a.lock.Lock()
	defer a.lock.Unlock()
	if a.size == 0 {
		return ""
	}
	return a.array[a.size]
}

func (a *ArrayStack)Top()  {
	return a.array[a.size-1]
}

// 入栈
func (stack *ArrayStack) Push(v string) {
    stack.lock.Lock()
    defer stack.lock.Unlock()

    // 放入切片中，后进的元素放在数组最后面
    stack.array = append(stack.array, v)

    // 栈中元素数量+1
    stack.size = stack.size + 1
}

func (stack *ArrayStack) Pop() string {
    stack.lock.Lock()
    defer stack.lock.Unlock()

    // 栈中元素已空
    if stack.size == 0 {
        panic("empty")
    }

    // 栈顶元素
    v := stack.array[stack.size-1]

    // 切片收缩，但可能占用空间越来越大
    //stack.array = stack.array[0 : stack.size-1]

    // 创建新的数组，空间占用不会越来越大，但可能移动元素次数过多
    newArray := make([]string, stack.size-1, stack.size-1)
    for i := 0; i < stack.size-1; i++ {
        newArray[i] = stack.array[i]
    }
    stack.array = newArray

    // 栈中元素数量-1
    stack.size = stack.size - 1
    return v
}

// 获取栈顶元素
func (stack *ArrayStack) Peek() string {
    // 栈中元素已空
    if stack.size == 0 {
        panic("empty")
    }

    // 栈顶元素值
    v := stack.array[stack.size-1]
    return v
}

// 栈大小
func (stack *ArrayStack) Size() int {
    return stack.size
}

// 栈是否为空
func (stack *ArrayStack) IsEmpty() bool {
    return stack.size == 0
}



func main()  {
	
}