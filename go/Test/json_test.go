package json_test

import(
	"encoding/json"
	"fmt"
	"testing"
)
type BasicInfo struct {
	Name string `json:"name"`
	Age  int    `json:"age"`
}
type JobInfo struct {
	Skills []string `json:"skills"`
}
type Employee struct {
	BasicInfo BasicInfo `json:"basic_info"`
	JobInfo   JobInfo   `json:"job_info"`
}

var jsonstr = `{
	"basic_info":{
		"name":"Mike",
	  "age":30
  },
  "job_info":{
	  "skills":["Java","Go","C"]
  }
} `

func TestEmbeddedJson(t *testing.T)  {
	e := new(Employee)
	err := json.Unmarshal([]byte(jsonstr),e)
	if err != nil{
		t.Error(err)
	}
	fmt.Println(*e)

	if v ,err := json.Marshal(e); err == nil{
		fmt.Println(string(v))
	}else{
		t.Error(err)
	}
}


type DataRecive interface {
}
func TestJsonEncodeAndDecode(t*testing.T)  {
	b := BasicInfo{"wamgming",21}
	arr := []string{"go","c++","elixir"}
	j := JobInfo{arr}
	e := Employee{b,j}
	str := string("") 
	t.Log(e)
	if v ,err := json.Marshal(e); err == nil{
		str = string(v)
	}else{
		t.Error(err)
	}
	fmt.Println(str)
	ee := new(DataRecive)//如果是接口，则转成map,map[basic_info:map[age:21 name:wamgming] job_info:map[skills:[go c++ elixir]]]
	if err := json.Unmarshal([]byte(str),ee);err == nil{//json必须要转成某种结构体吗？
		fmt.Println(*ee)
	}else{
		t.Error(err)
	}
}