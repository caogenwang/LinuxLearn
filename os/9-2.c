
#include<stdlib.h>
#include<stdio.h>

//关闭中断
void hal_cli()
{
    __asm__ __volatile__("cli": : :"memory");
}
//开启中断
void hal_sti()
{
    __asm__ __volatile__("sti": : :"memory");
}
//使用场景
void foo()
{
    hal_cli();
    //操作数据……
    hal_sti();
}
void bar()
{
    hal_cli();
    //操作数据……
    hal_sti();
}

typedef u32_t cpuflg_t;
static inline void hal_save_flags_cli(cpuflg_t* flags)
{
     __asm__ __volatile__(
            "pushfl \t\n" //把eflags寄存器压入当前栈顶
            "cli    \t\n" //关闭中断
            "popl %0 \t\n"//把当前栈顶弹出到flags为地址的内存中        
            : "=m"(*flags)
            :
            : "memory"
          );
}

static inline void hal_restore_flags_sti(cpuflg_t* flags)
{
    __asm__ __volatile__(
              "pushl %0 \t\n"//把flags为地址处的值寄存器压入当前栈顶
              "popfl \t\n"   //把当前栈顶弹出到flags寄存器中
              :
              : "m"(*flags)
              : "memory"
              );
}

int main(){

}