package main

import(
	"fmt"
)

type VisitorFunc func(*Info, error) error

type Visitor interface{//Visitor是一个泛型，接收多个中VisitorFunc
	Visit(VisitorFunc) error
}

type Info struct{
	Namespace string
	Name      string
	OtherThings string
}

func (info *Info)Visit(fn VisitorFunc) error {//实现了Visit，也是Visitor的一种
	return fn(info,nil)
}


type NameVisitor struct{
	visitor Visitor
}

func (v NameVisitor)Visit(fn VisitorFunc) error {//来包装VisitorFunc
	return v.visitor.Visit(func (info *Info,err error) error {
		fmt.Println("NameVisitor() before call function") 
		err = fn(info, err) 
		if err == nil { 
			fmt.Printf("==> Name=%s, NameSpace=%s\n", info.Name, info.Namespace) 
		} 
		fmt.Println("NameVisitor() after call function") 
		return err
	})
}

type OtherThingsVisitor struct{
	visitor Visitor
}

func (v OtherThingsVisitor)Visit(fn VisitorFunc) error {//来包装VisitorFunc
	return v.visitor.Visit(func (info *Info,err error) error {
		fmt.Println("OtherThingsVisitor() before call function") 
		err = fn(info, err) 
		if err == nil { 
			fmt.Printf("==> OtherThingsVisitor=%s\n", info.OtherThings) 
		} 
		fmt.Println("OtherThingsVisitor() after call function") 
		return err
	})
}

type LogVisitor struct{
	visitor Visitor
}

func (v LogVisitor)Visit(fn VisitorFunc) error {//来包装VisitorFunc
	return v.visitor.Visit(func (info *Info,err error) error {
		fmt.Println("LogVisitor() before call function") 
		err = fn(info, err) 
		fmt.Println("LogVisitor() after call function") 
		return err
	})
}

func main()  {
	info := Info{}
	var v Visitor = &info
	v = LogVisitor{v}
	v = NameVisitor{v} 
	// v = OtherThingsVisitor{v}//嵌套了好几层的函数

	loadFile := func(info *Info, err error) error { 
		info.Name = "Hao Chen" 
		info.Namespace = "MegaEase" 
		info.OtherThings = "We are running as remote team." 
		return nil 
	} 
	v.Visit(loadFile)
}


