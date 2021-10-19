package reflect

import (
	"fmt"
	"reflect"
	"testing"
)

func CheckType(v interface{})  {
	t := reflect.TypeOf(v)
	switch t.Kind() {
	case reflect.Float32, reflect.Float64:
		fmt.Println("Float")
	case reflect.Int, reflect.Int32, reflect.Int64:
		fmt.Println("Integer")
	default:
		fmt.Println("Unknown", t)
	}
}

func TestCheckType(t*testing.T)  {
	CheckType("10.0")
}

type employee struct{
	Id int
	Name string `format:"normal"`
	Age int
}

func (e *employee)UpdateAge(age int)  {
	e.Age = age
}
func (e *employee)UpdateName(name string)  {
	e.Name = name
}
// func TestReflect(t*testing.T)  {
// 	e := employee{1,"小王",18}
// 	t.Log(reflect.ValueOf(e).FieldByName("Name"))
// 	if nameField,ok := reflect.TypeOf(e).FieldByName("Name"); !ok {
// 		t.Error("Failed to get 'Name' field.")
// 	}else{
// 		t.Log("Tag:format",nameField.Tag.Get("format"))
// 	}
// 	// reflect.ValueOf(e).MethodByName("UpdateAge").Call([]reflect.Value{reflect.ValueOf(1)})
// 	reflect.ValueOf(&e).MethodByName("UpdateAge").//这个地方必须用指针
// 		Call([]reflect.Value{reflect.ValueOf(1)})
// 	t.Log("Updated Age:", e)
// }

func TestReflectNew(t*testing.T)  {
	e := employee{1,"小王",18}
	getType := reflect.TypeOf(e)
	fmt.Println(getType)
	getValue := reflect.ValueOf(e)
	fmt.Println(getValue)

	for i := 0; i < getType.NumField(); i++ {
		field := getType.Field(i)
		value := getValue.Field(i).Interface()
		fmt.Printf("%s: %v = %v\n", field.Name, field.Type, value)
	}
	// fmt.Println(getType.NumMethod())
	// for i := 0; i < getType.NumMethod(); i++ {
	// 	m := getType.Method(i)
	// 	fmt.Printf("%s: %v\n", m.Name, m.Type)
	// }
	reflect.ValueOf(&e).MethodByName("UpdateName").//这个地方必须用指针
		Call([]reflect.Value{reflect.ValueOf("小刘")})
	t.Log(e)
}