#ifndef MYPIPE_H
#define MYPIPE_H
#include <stdio.h>
#include <stdlib.h>
#define PIPESIZE 1024
#define MYPIPE_READ  0x00000001UL
#define MYPIPE_WRITE  0x00000002UL

typedef void mypipe_t;
void mypipe_register(mypipe_t*,int opmap);
void mypipe_unregister(mypipe_t*,int opmap);
mypipe_t *mypipe_init(void);
int mypipe_read(mypipe_t*,void *buf,size_t size);
int mypipe_write(mypipe_t*,const void *buf,size_t);
int mypipe_destroy(mypipe_t*);


#endif