#ifndef MYTBF_H
#define MYTBF_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

#define MYTBF_MAX 1024
typedef void mytbf_t;
typedef void (*sighandler_t) (int);

mytbf_t* mytbf_init(int cps,int burst);
int mytbf_fetchToken(mytbf_t* me,int size);
int mytbf_returnToken(mytbf_t* ptr,int size);
int mytbf_destory(mytbf_t* ptr);
#endif