#ifndef PROTO_H
#define PROTO_H

#define KEYPATH "test.txt"
#define KEYPROJ 'g'
enum
{
    MSG_PATH=1,
    MSG_DATA,
    MSG_EOF
};

typedef struct msg_path_st
{
    long mtype;
    char path[1024];
}msg_path_t;

typedef struct msg_data_st
{
    long mtype;
    char data[1024];
    int datasize;
}msg_data_t;

typedef struct msg_eot_st
{
    long mtype;
}msg_eot_t;

union msg_s2c_un
{
    long mtype;
    msg_data_t datamsg;
    msg_eot_t eotmsg;
};


#endif