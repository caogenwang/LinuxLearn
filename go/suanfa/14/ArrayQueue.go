package main

import "sync"

type ArrayQueue struct{
	array []string
	size   int
	lock   sync.Mutex
}

func (queue*ArrayQueue)Push(value string){
	queue.lock.Lock()
	defer queue.lock.Unlock()
	queue.array = append(queue.array,value)
	queue.size += 1
}


func (queue*ArrayQueue)Pop() string {
	if queue.size == 0{
		panic("empty")
	}
	queue.lock.Lock()
	defer queue.lock.Unlock()
	v := queue.array[0]
	
	// for i := 1; i < queue.size; i++ {
	// 	queue.array[i-1] = queue.array[i]
	// }
	// queue.array = queue.array[0 : queue.size-1]

	// 创建新的数组，移动次数过多
	newArray := make([]string, queue.size-1, queue.size-1)
	for i := 1; i < queue.size; i++ {
        // 从老数组的第一位开始进行数据移动
        newArray[i-1] = queue.array[i]
    }
    queue.array = newArray
	// 队中元素数量-1
	queue.size = queue.size - 1
	return v
}

func main(){

}