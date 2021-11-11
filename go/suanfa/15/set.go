package main

import(
	"sync"
	"fmt"
)

// 集合结构体
type Set struct {
    m            map[int]struct{} // 用字典来实现，因为字段键不能重复
    len          int              // 集合的大小
    sync.RWMutex                  // 锁，实现并发安全
}

func NewSet(Cap int) *Set {
	return &Set{
		m: make(map[int]struct{},Cap),
		len: 0,
	}
}

func (s*Set)Add(item int)  {
	s.Lock()
	defer s.Unlock()
	s.m[item] = struct{}{}
	s.len +=1
}

func (s *Set)Remove(item int)  {
	s.Lock()
	defer s.Unlock()
	if s.len == 1 {
		return
	}
	delete(s.m,item)
	s.len -= 1
}

func (s *Set)Has(item int) bool{
	s.Lock()
	defer s.Unlock()
	_,ok := s.m[item]
	return ok
}

func (s *Set)Size()int {
	return s.len
}

func (s *Set)Clear()  {
	s.Lock()
	defer s.Unlock()
	s.m = map[int]struct{}{} // 字典重新赋值
    s.len = 0                // 大小归零
}

func (s *Set) List() []int {
	s.Lock()
	defer s.Unlock()
	list := []int{}
	for k, _ := range s.m {
		list = append(list,k)
	}
	return list
}

func (s *Set)IsEmpty() bool {
	s.Lock()
	defer s.Unlock()
	return s.len == 0
}
func main() {
    //other()

    // 初始化一个容量为5的不可重复集合
    s := NewSet(5)

    s.Add(1)
    s.Add(1)
    s.Add(2)
    fmt.Println("list of all items", s.List())

    s.Clear()
    if s.IsEmpty() {
        fmt.Println("empty")
    }

    s.Add(1)
    s.Add(2)
    s.Add(3)

    if s.Has(2) {
        fmt.Println("2 does exist")
    }

    s.Remove(2)
    s.Remove(3)
    fmt.Println("list of all items", s.List())
}
