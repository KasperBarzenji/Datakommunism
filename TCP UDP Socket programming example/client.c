#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFERSIZE 1024

int main(int argc, char **argv) {
	if(argc != 2){
		printf("Usage: %s <port>", argv[0]);
		exit(0);
	}
	
    int sockfd;
    struct sockaddr_in server_addr; // Server address struct
	int port = atoi(argv[1]);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Sending request to server
    if (sendto(sockfd, NULL, 0, 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Send req failed");
        exit(EXIT_FAILURE);
    }
	
    time_t raw_time; // Time data
    int len = sizeof(server_addr);

    // Receive time
    if(recvfrom(sockfd, (char *)&raw_time, sizeof(raw_time), MSG_WAITALL, (struct sockaddr *)&server_addr, &len) < 0)
    {
        perror("Recv failed");
        exit(EXIT_FAILURE);
    }

    raw_time = ntohl(raw_time) - 2208988800UL;

    // Convert to readable format
    printf("Time received from server: %s", ctime(&raw_time));

    close(sockfd);

    return 0;
}