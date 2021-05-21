#ifndef MEDIALIB_H_
#define MEDIALIB_H_

struct mlibentry_st
{
    chnid_t chnid;
    char *desc;
};

int mlib_getchnlist(struct mlib_listentry**,int*);
int mlib_freechnlist(struct mlib_listentry*);

#endif