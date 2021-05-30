
#define SEM_FLG_MUTEX 0
#define SEM_FLG_MULTI 1
#define SEM_MUTEX_ONE_LOCK 1
#define SEM_MULTI_LOCK 0
//等待链数据结构，用于挂载等待代码执行流（线程）的结构，里面有用于挂载代码执行流的链表和计数器变量，这里我们先不深入研究这个数据结构。
typedef struct s_KWLST
{   
    spinlock_t wl_lock;
    uint_t   wl_tdnr;
    list_h_t wl_list;
}kwlst_t;
//信号量数据结构
typedef struct s_SEM
{
    spinlock_t sem_lock;//维护sem_t自身数据的自旋锁
    uint_t sem_flg;//信号量相关的标志
    sint_t sem_count;//信号量计数值
    kwlst_t sem_waitlst;//用于挂载等待代码执行流（线程）结构
}sem_t;