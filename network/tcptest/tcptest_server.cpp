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

int handle_message(int clientfd)
{
    char buf[MAXBUF + 1];
    bzero(buf, MAXBUF + 1);
    ssize_t n = recv(clientfd, buf, MAXBUF, 0);
    if (n > 0)
    {
        printf("==>  fd{%d} R: %s len=%d\n",clientfd, buf, int(n));
		ssize_t sn =send(clientfd, buf, n, 0);
		buf[sn]='\0';
		printf("<==  fd{%d} S: %s len=%d\n",clientfd, buf, int(sn));		
    }
    else if(n==0)
    {
        printf("XXX  Remote client has closed! recv return n=%d\n",int(n));
        close(clientfd);
    }
    else
    {
        printf("     Failed recved! errno=%d,%s\n",errno, strerror(errno));
        close(clientfd);
    }
    return n;
}


int main(int argc, char **argv)
{
	if(argc<2)
	{
		fprintf(stderr,"Usage:\n %s <port>\n",argv[0]);
		return 1;
	}
    int serverPort =atoi(argv[1]);
	if(serverPort<1)
	{
		fprintf(stderr,"serverPort{%d} error\n",serverPort);
		return 1;
	}
    
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    
    int listenerfd = socket(PF_INET, SOCK_STREAM, 0);
    if (-1==listenerfd)
    {
        printf("ERROR socket ,errno=%d,rt=%d,%s \n",errno,listenerfd,strerror(errno));
        return 0;
    }

	int opt=SO_REUSEADDR;
    setsockopt(listenerfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	
    setnonblocking(listenerfd);
    
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(serverPort);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenerfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) 
    {
        printf("ERROR bind ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    } 
    if (listen(listenerfd, 128) == -1) 
    {
        printf("ERROR listen ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }

    int efd = epoll_create(MAXEPOLLSIZE);
    socklen_t len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenerfd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listenerfd, &ev) < 0) 
    {
        printf("ERROR epoll_ctl ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }
    
    int maxevents = 1;
    while (maxevents>0) 
    {
        int n = epoll_wait(efd, events, maxevents, -1);
//		printf("      epoll_wait returned %d \n",n);
        if (n == -1)
        {
            printf("ERROR epoll_wait ,errno=%d,%s \n",errno,strerror(errno));
            return 0;
        }
     
        for (int i = 0; i < n; ++i)
        {
            if (events[i].data.fd == listenerfd) 
            {
                struct sockaddr_in their_addr;
                int clientfd = accept(listenerfd, (struct sockaddr *) &their_addr,&len);
                if (clientfd < 0) 
                {
                    printf("ERROR accept ,errno=%d,%s \n",errno,strerror(errno));
                    continue;
                } 
                else
                {
                    printf("     Connected from %s:%d， client socket:%d\n",
                            inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), clientfd);
                }
                setnonblocking(clientfd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientfd;
                if (epoll_ctl(efd, EPOLL_CTL_ADD, clientfd, &ev) < 0)
                {
                    printf("ERROR epoll_ctl ,errno=%d,%s \n",errno,strerror(errno));
                    return -1;
                }
                maxevents++;
            } 
            else
            {
                int rt = handle_message(events[i].data.fd);
                if (rt < 1 && errno != 11)
                {
                    epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd,&ev);
                    maxevents--;
                }
            }
        }
    }
    close(listenerfd);
    return 0;
}
