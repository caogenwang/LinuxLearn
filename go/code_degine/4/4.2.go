package main

import (
	"errors"
)
type Undo []func()

func (undo*Undo)Add(function func())  {
	*undo = append(*undo,function)
}
func (undo*Undo) Undo() error {
	functions := *undo
	if len(functions) == 0 {
		return errors.New("No functions to undo")
	}
	index := len(functions) - 1
	if function := functions[index]; function != nil{
		function()
		functions[index] = nil
	}
	*undo = functions[:index]
	return nil
}

type IntSet struct{
	set map[int]bool
	undo Undo
}
func NewIntSet() IntSet {
	return IntSet{set: make(map[int]bool)}
}

func (s *IntSet)Add(x int)  {
	if !s.Contain(x) {
		s.set[x] = true
		s.undo.Add(func () {s.Delete(x)})
	}else{
		s.undo.Add(nil)
	}
}

func (s *IntSet)Delete(x int)  {
	if s.Contain(x){
		delete(s.set,x)
		s.undo.Add(func(){s.Add(x)})
	}else{
		s.undo.Add(nil)
	}
}

func (s *IntSet)Contain(x int) bool {
	return s.set[x]
}

func (s*IntSet)Undo() error {
	return s.undo.Undo()
}

func main()  {
	
}