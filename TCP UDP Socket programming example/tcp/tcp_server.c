#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main()
{
	/* Declarations */
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int clientaddrlen;
	int request_sd, sd;
	char buf[13];

	/* Create socket */
	request_sd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

	printf("Servers listening socket created. \n");

	/* Fill in the address structure */
	memset(&serveraddr, 0,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(4446);
	/* Bind address to socket */
	bind(request_sd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr_in));
	printf("Servers address binded to the socket. \n");

	/* Activate connect request queue */
	listen(request_sd, SOMAXCONN);

	/* Receive connection */
	clientaddrlen =sizeof(struct sockaddr_in);
	sd = accept(request_sd,(struct sockaddr*)&clientaddr,&clientaddrlen);
	printf("Client connected. \n");

	/* Read data from socket and write it */
	read(sd, buf, 12);
	printf("Read text from the client. \n");
	buf[12] = '\0';
	printf("%s\n", buf);

	/* Send data back over connection */
	write(sd, buf, 12);
	printf("Send text back to the client. \n");

	/*Close sockets */
	close(sd); close(request_sd);
	printf("Both sockets are closed.");
}

