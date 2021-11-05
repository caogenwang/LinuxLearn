package main

import (
	"errors"
	"fmt"
	"reflect"
	"sync"
)

// IntStrMap 代表键类型为int、值类型为string的并发安全字
type IntStrMap struct {
	m sync.Map
}

func (iMap *IntStrMap) Delete(key int) {
	iMap.m.Delete(key)
}

func (iMap *IntStrMap) Load(key int) (value string, ok bool) {
	v, ok := iMap.m.Load(key)
	if v != nil {
		value = v.(string)
	}
	return
}

func (iMap*IntStrMap)LoadOrStore(key int,value string) (actual string, loaded bool) {
	a,loaded := iMap.m.LoadOrStore(key,value)
	actual = a.(string)
	return
}

func (iMap *IntStrMap) Range(f func(key int, value string) bool) {
	f1 := func(key, value interface{}) bool {
		return f(key.(int), value.(string))
	}
	iMap.m.Range(f1)
}

func (iMap *IntStrMap) Store(key int, value string) {
	iMap.m.Store(key, value)
}

// pairs 代表测试用的键值对列表。
var pairs = []struct {
	k int
	v string
}{
	{k: 1, v: "a"},
	{k: 2, v: "b"},
	{k: 3, v: "c"},
	{k: 4, v: "d"},
}

type ConcurrentMap2 struct{
	m         sync.Map
	keyType   reflect.Type
	valueType reflect.Type
}

func NewConcurrentMap2(keyType,valueType reflect.Type) (*ConcurrentMap,err) {
	if keyType == nil {
		return nil,errors.New("nil key type")
	}
	if !keyType.Comparable() {
		return nil,fmt.Errorf("incomparable key type: %s", keyType)
	}
	if valueType == nil {
		return nil, errors.New("nil value type")
	}

	cMap := &ConcurrentMap2{
		keyType: keyType,
		valueType: valueType,
	}
	return cMap,nil
}

func (cMap *ConcurrentMap2)Delete(key interface{})  {
	if reflect.TypeOf(key) != cMap.keyType {
		return
	}
	cMap.m.Delete(key)
}

func (cMap *ConcurrentMap2) Load(key interface{}) (value interface{}, ok bool){
	if reflect.TypeOf(key) !=  cMap.keyType{
		return
	}
	return cMap.m.Load(key)
}

func (cMap *ConcurrentMap2)LoadOrStore(key interface{},value interface{}) (actual interface{}, loaded bool){
	if reflect.TypeOf(key) != cMap.keyType {
		panic(fmt.Errorf("wrong key type: %v", reflect.TypeOf(key)))
	}
	if reflect.TypeOf(value) != cMap.valueType {
		panic(fmt.Errorf("wrong value type: %v", reflect.TypeOf(value)))
	}
	actual,loaded = cMap.m.LoadOrStore(key,value)
	return
}

func (cMap *ConcurrentMap2) Range(f func(key, value interface{}) bool) {
	cMap.m.Range(f)
}

func  (cMap *ConcurrentMap2)Store(key, value interface{})  {
	if reflect.TypeOf(key) != cMap.keyType {
		return
	}
	if reflect.TypeOf(value) != cMap.valueType {
		return
	}

	cMap.m.Store(key,value)
}


var pairs2 = []struct {
	k int
	v string
}{
	{k: 1, v: "a"},
	{k: 2, v: "b"},
	{k: 3, v: "c"},
	{k: 4, v: "d"},
}

func main()  {
	
	//1

}



