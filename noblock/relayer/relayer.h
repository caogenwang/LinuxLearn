#ifndef RELAYER_H
#define RELAYER_H
#include <stdio.h>
#include <stdlib.h>

enum
{
	STATE_RUNNING=1,
	STATE_CANCLED,
	STATE_OVER

};

struct rel_stat_st
{
	int state;
	int fd1;
	int fd2;
	int64_t count12,count21;
}

int rel_addjob(int fd1,int fd2)
{


}


int rel_canclejob(int id)
{

}

int rel_waitjob(int id,struct rel_stat_st *)
{

}



int rel_statjob(int id,struct rel_stat_st*)
{

}
#endif
