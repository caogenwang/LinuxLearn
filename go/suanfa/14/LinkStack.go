package main

import "sync"

type LinkStack struct{
	root *LinkNode
	size int
	lock sync.Mutex
}

type LinkNode struct{
	value int
	LinkNode *next
}

func (stack *LinkStack)Push(value int)  {
	stack.lock.Lock()
	defer stack.lock.Unlock()
	if stack.root == nil {//nil的时候是不能赋值的
		stack.root = new(LinkNode)
		stack.root.value = value
	}else{
		newNode := new(LinkNode)
		newNode.value = value
		newNode.next = stack.root
		stack.root = newNode
	}
	stack.size += 1
}

func (stack *LinkStack)Pop() int {
	if stack.size == 0 {
		panic("empty")
	}
	node := stack.root
	stack.lock.Lock()
	defer stack.lock.Unlock()
	value = node.value
	stack.root = node.next
	stack.size -=1 
	return value
}

func (stack *LinkStack)Top() int {
	if stack.size == 0 {
		return -1
	}
	stack.lock.Lock()
	defer stack.lock.Unlock()
	return stack.root.value
}