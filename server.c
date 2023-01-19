#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
	perror(msg);	//inputs error number and outputs using stderr
	exit(1);
}


int main(int argc, char *argv[])
{
	if(argc <2)
	{
		printf("port number not provided\n");
		exit(1);
	}
	int sockfd, newsockfd, portnum,n ;
	char buffer[255];

	struct sockaddr_in serv_addr , cli_addr;
	socklen_t clilen;	//socklen_t is a data type in socket.h
	sockfd =socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd <0)
	{
		error("Error opening socket");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));	//clears anything in whatever it is referenced to
	portnum=atoi(argv[1]);
	//atoi converts string to integer
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(portnum);	//htons is host to network short
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
		error("Binding failed");
	
	listen(sockfd, 5);
	clilen=sizeof(cli_addr);

	newsockfd= accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

	if (newsockfd<0)
		error("Error on accept");

	while(1)
	{
		bzero(buffer, 255);
		n=read(newsockfd, buffer,255);

		if(n<0)
			error("Error on reading");
		printf("Client : %s\n", buffer);
		bzero(buffer,255);
		fgets(buffer,255,stdin);

		n=write(newsockfd,buffer,strlen(buffer));
		if(n<0)
			error("Erro on writing");

		int i=strncmp("Bye",buffer,3);
		if(i==0)
			break;
	}

	close(newsockfd);
	close(sockfd);
	return 0;


}
