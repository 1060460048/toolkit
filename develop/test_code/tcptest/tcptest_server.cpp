#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#define MAXBUF 1024
#define MAXEPOLLSIZE 10000

int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
}

int handle_message(int new_fd)
{
    char buf[MAXBUF + 1];
    bzero(buf, MAXBUF + 1);
    ssize_t n = recv(new_fd, buf, MAXBUF, 0);
    if (n > 0)
    {
        printf("fd{%d} recved:%s,len=%d\n",new_fd, buf, int(n));
		ssize_t sn =send(new_fd, buf, n, 0);
		buf[sn]='\0';
		printf("fd{%d} sent:%s,len=%d\n",new_fd, buf, int(sn));		
    }
    else if(n==0)
    {
        printf("Remote client has closed!\n");
        close(new_fd);
    }
    else
    {
        printf("Failed recved! errno=%d,%s\n",errno, strerror(errno));
        close(new_fd);
    }
    return n;
}


int main(int argc, char **argv)
{
	if(argc<2)
	{
		fprintf(stderr,"Usage: %s port\n",argv[0]);
		return 1;
	}
    unsigned int myport =atoi(argv[1]);

    int listener, new_fd, kdpfd, nfds, n, ret, curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int lisnum;
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;
    lisnum = 2; 
	
    /* 设置每个进程允许打开的最大文件数 */
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) 
    {
        perror("setrlimit");
        exit(1);
    }
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

	int opt=SO_REUSEADDR;
    setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
    setnonblocking(listener);
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) 
    {
        perror("bind");
        exit(1);
    } 
    if (listen(listener, lisnum) == -1) 
    {
        perror("listen");
        exit(1);
    }

    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listener;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0) 
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);
        return -1;
    }
    curfds = 1;
    while (1) 
    {
        nfds = epoll_wait(kdpfd, events, curfds, -1);
		printf("epoll_wait returned %d \n",nfds);
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }
     
        for (n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == listener) 
            {
                new_fd = accept(listener, (struct sockaddr *) &their_addr,&len);
                if (new_fd < 0) 
                {
                    perror("accept");
                    continue;
                } 
                else
                {
                    printf("Connected from %s:%d， client socket:%d\n",
                            inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);
                }
                setnonblocking(new_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0)
                {
                    fprintf(stderr, "EPOLL_CTL_ADD '%d' failed ！%s\n",
                            new_fd, strerror(errno));
                    return -1;
                }
                curfds++;
            } 
            else
            {
                ret = handle_message(events[n].data.fd);
                if (ret < 1 && errno != 11)
                {
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);
                    curfds--;
                }
            }
        }
    }
    close(listener);
    return 0;
}