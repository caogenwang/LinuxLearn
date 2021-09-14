package buffer_channel

import (
	"errors"
	"fmt"
	"testing"
	"time"
)

type ReusableObj struct {
	Id   int
	Name string
}

type ObjPool struct {
	bufChan chan *ReusableObj //缓存可用对象
}

func NewObjPool(numOfObj int) *ObjPool {
	objPool := ObjPool{}
	objPool.bufChan = make(chan *ReusableObj, numOfObj)
	for i := 0; i < numOfObj; i++ {
		objPool.bufChan <- &ReusableObj{}
	}
	return &objPool
}

func (p *ObjPool) GetObj(timeout time.Duration) (*ReusableObj, error) {
	select {
	case ret := <-p.bufChan:
		return ret, nil
	case <-time.After(timeout): //超时控制
		return nil, errors.New("time out")
	}
}

func (p *ObjPool) ReleaseObj(obj *ReusableObj) error {
	select {
	case p.bufChan <- obj:
		return nil
	default:
		return errors.New("overflow")
	}
}

func TestObjPool(t *testing.T) {
	pool := NewObjPool(10)

	for i := 0; i < 20; i++ {
		if v, err := pool.GetObj(time.Second * 1); err != nil {
			t.Error(err)
		} else {
			v.Id = 20
			v.Name = "xiaoming"
			// fmt.Printf("%T\n", v)
			fmt.Println(v.Id, v.Name)
			if err := pool.ReleaseObj(v); err != nil {
				t.Error(err)
			}
		}
	}
	fmt.Println("Done")
}
