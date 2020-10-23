#ifndef RELAY_H
#define RELAY_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/fcntl.h>
typedef void  rel_stat_st ;

int rel_addjob(int fd1,int fd2);
/*
return >=  0;
       == -EINVAL;
       == -ENOSPC;
       == -ENOMEM;
*/
int rel_canclejob();
/*
return ==  0;
       == -EINVAL;
       ==  -EBUSY;
*/
int rel_waitjob();
/*
    return =0;
    -EINVAL
*/
int rel_startjob();

int rel_statjob();

void rel_destroyjob();

#endif