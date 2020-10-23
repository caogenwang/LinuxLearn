#ifndef CGI_SERVER_H
#define CGI_SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
#include <sys/poll.h>
#include "processpool.h"

class cgi_conn{
    public:
        cgi_conn(){}
        ~cgi_conn(){}
        void init(int epollfd,int sockfd,const sockaddr_in &client_addr)
        {
            m_epollfd = epollfd;
            m_sockfd = sockfd;
            m_address = client_addr;
            memset(m_buf,'\0',BUFFER_SIZE);
            m_read_idx = 0;
        }
        void process()
        {
            int idx = 0;
            int ret = -1;

            while (true)
            {
                idx = m_read_idx;
                ret = recv(m_sockfd,m_buf+idx,BUFFER_SIZE-1-idx,0);
                if(ret < 0)
                {
                    if (errno != EAGAIN)
                    {
                        removefd(m_epollfd,m_sockfd);
                    }
                    break;
                }
                else if(ret == 0)
                {
                    removefd(m_epollfd,m_sockfd);
                    break;
                }
                else //处理数据
                {
                    m_read_idx += ret;
                    printf("user content is %s\n",m_buf);
                    for ( ; idx < m_read_idx; idx++)
                    {
                        if ((idx >= 1) && (m_buf[idx -1] == '\r') && (m_buf[idx] == '\n'))
                        {
                            break;
                        }
                    }
                    if (idx == m_read_idx)
                    {
                        continue;
                    }
                    m_buf[idx - 1] = '\0';

                    char *file_name = m_buf;
                    if (access(file_name,F_OK) == -1)//检查调用进程是否可以对指定的文件执行某种操作
                    {
                        removefd(m_epollfd,m_sockfd);
                        break;
                    }
                    pid_t pid = fork();
                    if(pid < 0)
                    {
                        removefd(m_epollfd,m_sockfd);
                        break;
                    }
                    else if(pid > 0)//父进程关闭连接
                    {
                        removefd(m_epollfd,m_sockfd);
                        break;
                    }
                    else
                    {
                        close(STDOUT_FILENO);
                        dup(m_sockfd);
                        execl(m_buf,m_buf,0);
                        exit(0);
                    }
                }  
            }
        }
    private:
        static const int BUFFER_SIZE = 1024;
        static int m_epollfd;
        int m_sockfd;
        sockaddr_in m_address;
        char m_buf[BUFFER_SIZE];
        int m_read_idx;
};

int cgi_conn::m_epollfd = -1;

#endif