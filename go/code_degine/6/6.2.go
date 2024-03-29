package main

import(
	"reflect"
)

type Container2 struct{
	s reflect.Value
}

func NewContainer(t reflect.Type,size int) *Container2{
	if size <=0 { 
		size=64 
	}
	return &Container2{
		s: reflect.MakeSlice(reflect.SliceOf(t),0,size),
	}
}

func (c*Container2)Put(val interface{}) error {
	if reflect.ValueOf(val).Type() != c.s.Type().Elem(){
		return fmt.Errorf("Put: cannot put a %T into a slice of %s", val, c.s.Type().Elem()))
	}
	c.s = reflect.Append(c.s, reflect.ValueOf(val)) 
	return nil
}

func (c*Container2)Get(refval interface{}) error {
	if reflect.ValueOf(refval).Kind() != reflect.Ptr || 
	reflect.ValueOf(refval).Elem().Type() != c.s.Type().Elem(){
		return fmt.Errorf("Get: needs *%s but got %T", c.s.Type().Elem(), refval)
	}
	reflect.ValueOf(refval).Elem().Set( c.s.Index(0) ) 
	c.s = c.s.Slice(1, c.s.Len()) 
	return nil
}


func main()  {
	f1 := 3.1415926
	f2 := 1.41421356237

	c := NewMyContainer(reflect.TypeOf(f1),16)
	if err := c.Put(f1);err != nil {
		panic(err)
	}
	if err := c.Put(f2);err != nil{
		panic(err)
	}

	g := 0.0
	if err := c.Get(&g);err != nil {
		panic(err)
	}
	fmt.Printf("%v (%T)\n", g, g) //3.1415926 (float64)
	fmt.Println(c.s.Index(0)) //1.4142135623

}