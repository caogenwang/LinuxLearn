package main

import(
	"fmt"
	"strings"
)

func MapStrtoStr(arr []string,fn func(s string) string)[]string{
	str := []string{}
	for _, v := range arr {
		str = append(str,fn(v))
	}
	return str
}

func MapStrtoInt(arr []string,fn func(s string)int)[]int {
	var intarr = []int{}
	for _, it := range arr {
		intarr = append(intarr,fn(it))
	}
	return intarr
}

func Reduce(arr []string,fn func(s string)int) int {
	var sum int = 0
	for _, it := range arr {
		sum += fn(it)
	}
	return sum
}

func Filter(arr []string,fn func(s string)bool) []string {
	newarr := []string{}
	for _, it := range arr {
		if !fn(it) {
			newarr = append(newarr,it)
		}
	}
	return newarr
}

type Employee struct {    
	Name     string    
	Age      int    
	Vacation int    
	Salary   int
}

func EmployeeCountIf(list []Employee,fn func(e*Employee)bool) int {
	count := 0
	for i, _ := range list {
		if fn(&list[i]){
			count += 1
		}
	}
	return count
}
func EmployeeFilterIn(list []Employee, fn func(e *Employee) bool) []Employee { 
	var newList []Employee 
	for i, _ := range list {
		 if fn(&list[i]) { 
			 newList = append(newList, list[i]) 
		} 
	} 
	return newList
}
func EmployeeSumIf(list []Employee, fn func(e *Employee) int) int { 
	var sum = 0 
	for i, _ := range list { 
		sum += fn(&list[i]) 
	} 
	return sum
}




func main()  {
	str := []string{"hao","jia","huo","j"}

	x := MapStrtoStr(str,func (s string)string  {//注意这种函数式编程
		return strings.ToUpper(s)
	})
	fmt.Println(x)
	
	y := MapStrtoInt(str,func (s string)int  {
		return len(s)
	})
	fmt.Println(y)

	z := Reduce(str,func(s string)int{
		return len(s)
	})
	fmt.Println(z)

	w := Filter(str,func(s string)bool{
		return len(s) == 1
	})
	fmt.Println(w)


	var list = []Employee{    
		{"Hao", 44, 0, 8000},    
		{"Bob", 34, 10, 5000},    
		{"Alice", 23, 5, 9000},    
		{"Jack", 26, 0, 4000},    
		{"Tom", 48, 9, 7500},    
		{"Marry", 29, 0, 6000},    
		{"Mike", 32, 8, 4000},
	}


}