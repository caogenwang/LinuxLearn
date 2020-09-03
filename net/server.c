#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#define SERV_PORT 6666

#if 0
struct sockaddr_in {
	__uint8_t	sin_len;
	sa_family_t	sin_family;
	in_port_t	sin_port;
	struct	in_addr sin_addr;
	char		sin_zero[8];
};

struct sockaddr {
	__uint8_t	sa_len;		/* total length */
	sa_family_t	sa_family;	/* [XSI] address family */
	char		sa_data[14];	/* [XSI] addr value (actually larger) */
};
#endif

int main()
{
    int lisfd,cfd;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    char buf[BUFSIZ], clie_IP[BUFSIZ];

    lisfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lisfd < 0)
    {
        perror("socket");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);  

    if (bind(lisfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
        perror("socket");
        exit(1);
    }

    if (listen(lisfd, 2) <0 ) 
    {
        perror("socket");
        exit(1);
    }
    printf("wait for client connect ...\n");

    clie_addr_len = sizeof(clie_addr_len);
    cfd = accept(lisfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
    {
        perror("accept");
        exit(1);
    }

    printf("cfd = ----%d\n", cfd);

    printf("client IP: %s  port:%d\n", 
            inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clie_addr.sin_port));
    int len,i;
    while (1)
    {
        len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);

        for (i = 0; i < len; i++)
            buf[i] = toupper(buf[i]);
        write(cfd, buf, len); 
    }
    
    close(lisfd);
    close(cfd);
}