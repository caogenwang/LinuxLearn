package obj_pool

import (
	"fmt"
	"time"
	"testing"
	"errors"
)

type ReuseObj struct{

}

type ObjPool struct{
	bufChan chan *ReuseObj//用于缓冲可重用对象
}

func NewObjPool() *ObjPool {
	objPool := ObjPool{}
	objPool.bufChan = make(chan *ReuseObj, 10)
	for i := 0; i < 10; i++ {
		objPool.bufChan <- &ReuseObj{}
	}
	return &objPool
}

func (obj *ObjPool)GetObj(timeout time.Duration) (*ReuseObj,error)  {
	select {
	case ret := <-obj.bufChan:
		return ret,nil
	case <-time.After(timeout):
		return nil,errors.New("time out")
	}
}

func (obj *ObjPool)ReleaseObj(o *ReuseObj) error {
	select {
	case obj.bufChan <- o:
		return nil
	default:
		return errors.New("overflow")
	}
}

func TestObjPool(t*testing.T)  {
	pool := NewObjPool()
	for i := 0; i < 12; i++ {
		if v, err := pool.GetObj(time.Second * 1); err != nil {
			t.Error(err)
		} else {
			fmt.Println(&v)
			fmt.Printf("%T\n", v)
			if err := pool.ReleaseObj(v); err != nil {
				t.Error(err)
			}
		}
	}
}