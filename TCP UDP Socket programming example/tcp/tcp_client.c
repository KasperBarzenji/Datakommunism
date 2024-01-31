#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main()
{
	/* Declarations */
	struct sockaddr_in serveraddr;
	int sd;
	char buf[13];

	/* Create socket */
	sd = socket(PF_INET,
                SOCK_STREAM,
	              IPPROTO_TCP);

	printf("Client socket created.. \n");

	/* Clear address structure */
	memset(&serveraddr, 0,
	      sizeof(struct sockaddr_in));

	/* Add address family */
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET,"10.91.124.207" ,
	          &serveraddr.sin_addr);
	/* Add the port number */
	serveraddr.sin_port = htons(4446);

	/* Connect */
	connect(sd,
	        (struct sockaddr*)&serveraddr,
	        sizeof(struct sockaddr_in));
	
	printf("Connected to the server.. \n");

	/* Send data */
	write(sd, "Hello world!", 12 );
	
	printf("Send text to the server.. \n");

	/* Read data */
	read(sd, buf, 12 );
	printf("Received text from the server.. \n");

	/* add string end sign, write to screen*/
	buf[12] = '\0';
	printf("%s\n", buf);

	/* Close socket */
	close(sd);
}

