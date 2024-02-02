#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFERSIZE 1024
#define MAXSIZE 4

int main(int argc, char **argv) {
    if(argc != 2){
		printf("Usage: %s <port>", argv[0]);
		exit(0);
	}

	int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr, client_addr; // Address structs
    time_t raw_time; // Time data

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // Bind the socket to the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
	else
	{
		printf("Socket bound to server address\n");
	}

    printf("Server listening on port %d...\n", port);

    // Server loop
    while (1) {
        int len, n;
        char buffer[BUFFERSIZE];

        len = sizeof(client_addr);

        // Receive client request
        n = recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0';

        time(&raw_time);

        // Convert time to RTF standard
        raw_time = htonl(raw_time + 2208988800UL);

        // Send time to client
        sendto(sockfd, (const char *)&raw_time, MAXSIZE, 0, (const struct sockaddr *)&client_addr, len);

        printf("Response sent to client\n");
    }

    return 0;
}