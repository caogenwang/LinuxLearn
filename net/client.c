#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666

int main()
{
    int cfd, len;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ]; 

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0)
    {
        perror("socket");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));                       
    serv_addr.sin_family = AF_INET;                             
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    
    serv_addr.sin_port = htons(SERV_PORT);  
    
    if(connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(1);
    }

     while (1) {
        fgets(buf, sizeof(buf), stdin);
        int r = write(cfd, buf, strlen(buf));       
        printf("Write r ======== %d\n", r);
        len = read(cfd, buf, sizeof(buf));
        printf("Read len ========= %d\n", len);
        write(STDOUT_FILENO, buf, len);
    }

    close(cfd);
    exit(0);
}