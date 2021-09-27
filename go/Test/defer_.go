package main
import(
	"fmt"
)
func main(){
	/*
	先defer的后执行
	recover后输出panic中的信息
	*/
	defer func(){
		if err:=recover();err!=nil{
			fmt.Println("yes")
			fmt.Print(err)
		} else {
			fmt.Print("no")
		}
	}()

	defer func(){
		fmt.Println("--------")
		panic("1111111111111")
	}()

	fmt.Println("********")
	panic("22222222222")
}
//输出
// ********
// --------
// yes
// 1111111111111