#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXDATASIZE 1024 // max number of bytes we can get at once


int main(int argc, char *argv[])
{
	int sockfd;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; // connector'saddress information

	if (( argc == 1) || (argc==2) ) 
	{
		fprintf(stderr,"usage: client hostname\nEx:\n$./client01 ip port\n");
		return 1;
	}

	if ((he=gethostbyname(argv[1])) == NULL) 
	{ // get the host info
		herror("gethostbyname");
		return 1;
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		return 1;
	}
	
	their_addr.sin_family = AF_INET; // host byte order

	their_addr.sin_port = htons(atoi(argv[2])); // short, network byte order

	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	if (connect(sockfd, (struct sockaddr *)&their_addr,sizeof their_addr) == -1) 
	{
		perror("connect");
		return 1;
	}

	
	for(int i=0;i<10;i++)
	{
		memset(buf,0,MAXDATASIZE);
		int nf =snprintf(buf,MAXDATASIZE,"Hello,i=%d",i);
		ssize_t n =send(sockfd, buf, nf, 0 );
		if(  n== -1 ) 
		{
			perror("send");
		}
		buf[n] ='\0';
		printf("Sent:%s,len=%d\n",buf,int(n));

		memset(buf,0,MAXDATASIZE);
		if ((n=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) 
		{
			perror("recv");
		}
		else if( n == 0 )
		{
			printf("Remote server has closed!\n");
			break;
		}
		buf[n] = '\0';
		printf("Recved:%s,len=%d\n",buf,int(n));
		sleep(8	);
	}
	close(sockfd);
	return 0;
}
