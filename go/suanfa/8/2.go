package main
import "fmt"

func Rescuvie(n int) int {
    if n == 0 {
        return 1
    }

    return n * Rescuvie(n-1)
}


//尾递归
func RescuvieTail(n,a int) int{
    if n == 1 {
        return a
    }
    return RescuvieTail(n-1,a*n)
}
//尾递归和递归的区别：递归函数中要做计算，n * Rescuvie(n-1)，
//而尾递归递归函数在调用自身后直接传回其值，而不对其再加运算，效率将会极大的提高
//尾递归函数，部分高级语言编译器会进行优化，减少不必要的堆栈生成，使得程序栈维持固定的层数，不会出现栈溢出的情况
//斐波那契数列
func F(n,a1,a2 int) int{
    if n == 1{
        return a1
    }
    return F(n-1,a2,a1+a2)
}
// 二分查找递归解法
func BinarySearch(array []int,target int ,l,r int) int {
    if l > r {
        return -1
    }
    mid := (l + r)/2
    if array[mid] == target {
        return array[mid]
    }else if array[mid] > target{
        return BinarySearch(array,target,l,mid - 1)
    }else{
        return BinarySearch(array,target,mid+1,r)
    }
    return -1
}

func main() {
    fmt.Println(RescuvieTail(5,1))
}