package main

import (
	"fmt"
)
//引用类型的空值是nil
// nil的时候是不能赋值的
// 二叉树
type TreeNode struct {
    Data  string    // 节点用来存放数据
    Left  *TreeNode // 左子树
    Right *TreeNode // 右字树
}

func preOrder(TreeNode *root){
	if root == nil{
		return	
	}
	fmt.Println(root.Data)
	if root.Left != nil {
		preOrder(root.Left)
	}
	if root.Right != nil{
		preOrder(root.Right)
	}
}

