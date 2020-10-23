#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

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

static int setnoblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd,int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,event);
    setnoblocking(fd);
}

int removefd(int epollfd,int fd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

static void sig_handler(int sig)
{
    int save_error = errno;
    int msg = sig;
    send(sig_pipefd[1],(char*)&msg,1,0);
    errno = save_error;
}

static void addsig(int sig,void(handler)(int),bool restart = true)
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = handler;
    if(restart)
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);//initializes a signal set to contain all signals
    assert(sigaction(sig,&sa,NULL) != -1);
}

class Process{

public:
    Process():m_pid(-1){}
private:
    pid_t m_pid;
    int m_pipe[2];
};

template<typename T>
class ProcessPool
{
private:
    ProcessPool(int listenfd,int process_number = 8);
public:
    static ProcessPool<T> *create(int listenfd,int process_number)
    {
        if (!m_instance)
        {
            m_instance = new ProcessPool<T>(listenfd,process_number);
        }
        return m_instance;
    }

    ~ProcessPool()
    {
        delete []m_sub_process;
    }
    void run();

private:
    void setup_sig_pipe();
    void run_parent();
    void run_child();

private:
    static const int MAX_PROCESS_NUMBER = 16;
    static const int USER_PER_PROCESS = 16;
    static const int MAX_EVENT_NUMBER = 16;
    int m_process_number;
    int m_idx;
    int m_epollfd;
    int m_listenfd;
    int m_stop;
    Process *m_sub_process;
    static ProcessPool<T>*m_instance;
};
template<typename T>
ProcessPool<T>*ProcessPool<T>::m_instance = nullptr;

static int sig_pipefd[2];


template<typename T>
ProcessPool<T>::ProcessPool(int listenfd,int process_number = 8):
m_listenfd(listenfd),m_process_number(process_number),m_idx(-1),
m_stop(false)
{
    assert((process_number > 0) && (process_number < MAX_PROCESS_NUMBER));
    m_sub_process = new Process[process_number];

    assert(m_sub_process);

    for (int i = 0; i < process_number; i++)
    {
        int ret = socketpair(PF_UNIX,SOCK_STREAM,0,m_sub_process[i].m_pipe);
        assert(ret == 0);
        m_sub_process[i].m_pid = fork();
        if (m_sub_process[i].m_pid > 0)
        {
            close(m_sub_process[i].m_pipe[1]);
            continue;
        }
        else
        {
            close(m_sub_process[i].m_pipe[0]);
            m_idx = i;
            break;
        }
    }
}

/*统一事件源,将所有的事件都放到epoll中监听*/
template<typename T>
void ProcessPool<T>::setup_sig_pipe()
{
    m_epollfd = epoll_create(5);
    assert(m_epollfd != -1);

    int ret = socketpair(PF_UNIX,SOCK_STREAM,0,sig_pipefd);
    assert(ret != -1);

    setnoblocking(sig_pipefd[1]);//信号管道，读管道为非阻塞
    addfd(m_epollfd,sig_pipefd[0]);//监听信号管道的写事件

    addsig(SIGCHLD,sig_handler);
    addsig(SIGTERM,sig_handler);
    addsig(SIGINT,sig_handler);
    addsig(SIGPIPE,SIG_IGN);
}

template<typename T>
void ProcessPool<T>::run()
{

    if(m_idx != -1)//存在子线程则运行子线程
    {
        run_child();
        return;
    }
    run_parent();
}

template<typename T>
void ProcessPool<T>::run_child()
{
    setup_sig_pipe();//信号注册

    int pipefd = m_sub_process[m_idx].m_pipe[1];

    addfd(m_epollfd,pipefd);//子进程负责接受连接，父进程只负责监听

    epoll_event events[MAX_EVENT_NUMBER];
    T *users = new T[USER_PER_PROCESS];//每个连接多少客户端

    assert(users);
    int number = 0;
    int ret = -1;

    while (!m_stop)
    {
        number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
        if((number < 0) && (errno != EINTR))
        {
            printf("epoll failed\n");
            break;
        }
        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if ((sockfd == pipefd) && (events[i].events & EPOLLIN))
            {
                int client = 0;
                ret = recv(sockfd,(char*)&client,sizeof(client),0);
                if(((number < 0) && (errno != EINTR)) || ret == 0)
                {
                    continue;//继续监听
                }
                else
                {
                    struct sockaddr_in client_address;
                    socklen_t client_addrlength = sizeof(client_address);
                    int connfd = accept(m_listenfd,(struct sockaddr*)&client_address,&client_addrlength);
                    if(connfd < 0)
                    {
                        perror("accept");
                        continue;
                    }
                    addfd(m_epollfd,connfd);//这个地方如果再有数据可读，则到分支3中
                    users[connfd].init(m_epollfd,connfd,client_address);
                } 
            }
            else if((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret = recv(sig_pipefd[0],signal,sizeof(signal),0);
                if(ret < 0)
                {
                    continue;
                }
                else
                {
                    for (int i = 0; i < ret; i++)
                    {
                        switch (signal[i])
                        {
                            case SIGCHLD:
                                pid_t pid;
                                int stat;
                                while ((pid=wait(-1,&stat,WNOHANG)) > 0)
                                {
                                    continue;
                                }
                            case SIGTERM:
                            case SIGINT:
                            {
                                m_stop = true;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                    
                }
            }
            else if(events[i].events & EPOLLIN)
            {
                users[sockfd].process();//数据的读取和逻辑处理都在子线程
            }
            else
            {
                continue;
            } 
        }
        
    }
    delete []users;
    users = nullptr;
    close(pipefd);
    close(m_epollfd);
}

template<typename T>
void ProcessPool<T>::run_parent()//主进程是同步监听，子进程是异步处理，这是一种reactor模式
{
    setup_sig_pipe();//信号注册

    addfd(m_epollfd,m_listenfd);
    epoll_event events[MAX_EVENT_NUMBER];

    int sub_process_counter = 0;
    int new_conn = 1;
    int number = 0;
    int ret = -1;

    while (!m_stop)
    {
        number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
        if((number < 0) && (errno != EINTR))
        {
            printf("epoll failed\n");
            break;
        }
        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == m_listenfd)
            {

            }
            else if((sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret = recv(sig_pipefd[0],signal,sizeof(signal),0);
                if(ret < 0)
                {
                    continue;
                }
                else
                {
                    for (int i = 0; i < ret; i++)
                    {
                        switch (signal[i])
                        {
                            case SIGCHLD:
                            {
                                while ((pid=wait(-1,&stat,WNOHANG)) > 0)//回收退出的子进程
                                {
                                    for (int i = 0; i < m_process_number; i++)
                                    {
                                        if(m_process_number[i].m_pid = pid)
                                        {
                                            printf("child %d join\n",i);
                                            close(m_sub_process[i].m_pipe[0]);
                                            m_sub_process[i].m_pid = -1;
                                        }
                                    }
                                }
                                m_stop = true;
                                for (int i = 0; i < m_process_number; i++)
                                {
                                    if(m_process_number[i].m_pid == -1)
                                    {
                                        m_stop = false;
                                    }
                                }
                                break;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {   //kill all the child process
                                printf("kill all the child process\n");
                                 for (int i = 0; i < m_process_number; i++)
                                {
                                    pid_t pid = m_process_number[i].m_pid;
                                    if(pid != -1)
                                    {
                                        kill(pid,SIGTERM);
                                    }
                                }
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }

            }
            else
            {
                continue;
            }
            
        }
    }
    close(m_epollfd);
}
#endif 



//1. pid>0时，只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。
//2. pid=-1时，等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。
//3. pid=0时，等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬。
//4. pid<-1时，等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值。