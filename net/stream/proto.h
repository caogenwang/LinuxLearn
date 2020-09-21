#ifndef PROTO_H
#define PROTO_H

#define FMT_STAMP  "%lld\r\n"
#define SERVERPORT "1989"

struct msg_st
{
    uint8_t name[BUFSIZ];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));

#endif
