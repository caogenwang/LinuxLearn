#include<stdlib.h>
#include<stdio.h>

typedef struct memory_pool_s{
    char *start;
    char *end;
    char *data;
    int len;
}memory_pool_t;

typedef struct block{
    int a;
    int b;
}block_t;
memory_pool_t *free_list;

int main(){
    free_list = (memory_pool_t*)malloc(sizeof(block_t)*20 + sizeof(memory_pool_t));
    free_list->data = (char*)free_list + sizeof(memory_pool_t);
    free_list->start = free_list->data;
    free_list->end = free_list->data + 20*sizeof(block_t);
    free_list->len = 20*sizeof(block_t);
    int count = 0;
    while (free_list->start != free_list->end)
    {
        block_t *b1 = (block_t *)free_list->start;
        free_list->start = free_list->start + sizeof(block_t);
        free_list->len = free_list->len - sizeof(block_t);
        b1->a = 0;
        b1->b = 10;
        
        count ++;
    }
    printf("s: %p\n",free_list->start);
    printf("e: %p\n",free_list->end);
    printf("count:%d\n",count);
    
    return 0;
}