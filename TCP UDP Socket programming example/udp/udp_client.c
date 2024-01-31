#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s <port>", argv[0]);
		exit(0);
	}

	int port = atoi(argv[1]);
	int sockfd;
	struct sockaddr_in serverAddr;
	char buffer [1024];
	socklen_t addr_size;

	sockfd=socket(PF_INET, SOCK_DGRAM, 0);
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(port);
    inet_pton(AF_INET, "10.91.124.207",&serverAddr.sin_addr);

	strcpy(buffer, "Hello Server\n");
 	sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	printf("[+]Data send: %s", buffer);
	
	addr_size=sizeof(serverAddr);
	recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, &addr_size);
	printf("[+]Data received: %s", buffer);
	
}
