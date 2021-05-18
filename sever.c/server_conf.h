#ifndef SERVER_CONF_H_
#define SERVER_CONF_H_

#define DEFAULT_MEDIADIR   "/var/media/"
#define DEFAULT_IF         "eth0"
enum
{
    RUN_DAEMON=1,
    RUN_FOREGROUD
};
struct server_conf_st
{
    char *rcvport;
    char *mgroup;
    char *media_dir;
    char runmode;
    char *ifname;
};
extern struct server_conf_st server_conf;

#endif