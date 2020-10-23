#ifndef PROTO_H
#define PROTO_H

#define KEYPATH "test.txt"
#define KEYPROJ 'g'

struct msg_st
{
    long mtype;
    char name[512];
    int math;
    int chinese;
};

#endif