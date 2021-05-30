
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

//定义一个原子类型
typedef struct s_ATOMIC{
    volatile int a_count; //在变量前加上volatile，是为了禁止编译器优化，使其每次都从内存中加载变量
}atomic_t;
//原子读
static inline int atomic_read(const atomic_t *v)
{        
        //x86平台取地址处是原子
        return (*(volatile int*)&(v)->a_count);
}
//原子写
static inline void atomic_write(atomic_t *v, int i)
{
        //x86平台把一个值写入一个地址处也是原子的 
        v->a_count = i;
}
//原子加上一个整数
static inline void atomic_add(int i, atomic_t *v)
{
        __asm__ __volatile__("lock;" "addl %1,%0"
                     : "+m" (v->a_count)
                     : "ir" (i));
}
//原子减去一个整数
static inline void atomic_sub(int i, atomic_t *v)
{
        __asm__ __volatile__("lock;" "subl %1,%0"
                     : "+m" (v->a_count)
                     : "ir" (i));
}
//原子加1
static inline void atomic_inc(atomic_t *v)
{
        __asm__ __volatile__("lock;" "incl %0"
                       : "+m" (v->a_count));
}
//原子减1
static inline void atomic_dec(atomic_t *v)
{
       __asm__ __volatile__("lock;" "decl %0"
                     : "+m" (v->a_count));
}

atomic_t g_value={0};
void *tfun(void){
    for (int i = 0; i < 100000; i++)
    {
        atomic_inc(&g_value);
    }
    return 0;
}

int main(){
    pthread_t tid[2];
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&tid[i],NULL,(void*)tfun,NULL);
    }
    for (int i = 0; i < 2; i++)
    {
        pthread_join(tid[i],NULL);
    }
    printf("g_value:%d\n",g_value.a_count);
    return 0;
}