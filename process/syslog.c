#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main()
{
    
    opensyslog("mydaemon",LOG_PID,LOG_DAEMON);

    syslog(LOG_INFO,"daemonize() success!");
    
}
