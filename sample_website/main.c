#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

#define	PORT 8080

// https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void toLowerString(char *str)
{
    int len = strlen(str);
    for(int i = 0; i < len; i++)
    {
        str[i] = tolower(str[i]);
    }
}

int send_html(int socket)
{

	char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Read the request from the client
    bytes_read = read(socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        return -1;
    }

    // Null-terminate the received data
    buffer[bytes_read] = '\0';

    // Extract the filename from the HTTP request
    char filename[100];
    sscanf(buffer, "GET /%s ", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL || access(filename, F_OK) != 0) {
        // If file not found, return 404 Not Found response
        char not_found_response[] = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        write(socket, not_found_response, strlen(not_found_response));
        close(socket); // Close the client socket
        return -1;
    }

    // If file found, calculate its size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *ext = get_filename_ext(filename);
    toLowerString(ext);
    char content_type[100];
    if (strcmp(ext, "html") == 0) {
        strcpy(content_type, "Content-Type: text/html");
    } else {
        strcpy(content_type, "Content-Type: image/jpeg");
    }

    // Prepare the response headers with Content-Length
    char ok_response[1000];
    snprintf(ok_response, sizeof(ok_response), "HTTP/1.1 200 OK\r\n%s\r\nContent-Length: %ld\r\n\r\n", content_type, file_size);
    write(socket, ok_response, strlen(ok_response));

    // Send file contents
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        write(socket, buffer, bytes_read);
    }

    fclose(file);
    close(socket); // Close the client socket
    return 0;
}

int main()
{
	/* Declarations */
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int clientaddrlen;
	int request_sd, sd;
	char buf[BUFFER_SIZE];

	/* Create socket */
	request_sd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	printf("Servers listening socket created. \n");

	if (sd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

	/* Fill in the address structure */
	memset(&serveraddr, 0,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);
	/* Bind address to socket */
	if(bind(request_sd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr_in)) < 0)
	{
		perror("Error binding socket");
        exit(EXIT_FAILURE);
	}
	printf("Servers address binded to the socket. \n");

	/* Activate connect request queue */
	if (listen(request_sd, SOMAXCONN) < 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

	/* Receive connection */
	
	// Accept incoming connections
    while (1) {
       	clientaddrlen =sizeof(struct sockaddr_in);
	    sd = accept(request_sd,(struct sockaddr*)&clientaddr,&clientaddrlen);
        if (sd < 0) {
            perror("Error accepting connection");
            continue;
        }

        printf("Client connected\n");

        // Handle client request
        if(send_html(sd) < 0)
		{
			perror("Error sending file");
		}
    }

    // Close server socket
    close(sd);
	printf("Both sockets are closed.");
	
	return 0;
}
