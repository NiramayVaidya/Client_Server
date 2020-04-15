// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define DEBUG 0
#define ALTERNATE 1
#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
	long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello_terminal = "Hello from server";
	
	// char *hello_browser = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHelloWorld!";
	
	int fd = open("info.html", O_RDONLY);
	char *file_content;
	int file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, 0);
	char *hello_temp = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	int len = strlen(hello_temp);
	char *hello_browser = (char *)malloc(sizeof(char) * (len + 2 + file_size + 1));
	hello_browser[len] = '\0';
	int i = 0;
	while(i < len) {
		hello_browser[i] = hello_temp[i];
		i++;
	}
	//strcpy(hello_browser, hello_temp);
	len = 0;
	int digit;
	char *d = (char *)malloc(sizeof(char) * 2);
	d[1] = '\0';
	int fs = file_size;
	do {
		fs /= 10;
		len++;
	} while (fs);
	len -= 1;
	while (len >= 0) {
		digit = (int)(file_size / pow(10, len)) % 10;
		d[0] = '0' + digit;
		strcat(hello_browser, d);
		len -= 1;
	}
	free(d);
    file_content = (char *)malloc(sizeof(char) * (file_size + 1));
	file_content[file_size] = '\0';
#if ALTERNATE
	i = 0;
	char c;
	while(i < file_size) {
		read(fd, &c, sizeof(char));
		file_content[i] = c;
		i++;
	}
#endif
	//read(fd, file_content, file_size);
	strcat(hello_browser, "\n\n");
	strcat(hello_browser, file_content);
#if DEBUG
	printf("%s\n", hello_browser);
#endif
	close(fd);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    
    
    if (bind(server_fd, (struct sockaddr *)&address, (socklen_t)addrlen) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
	i = 2;
    while(i--) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
		if (strstr(buffer, "client") != NULL) {
			write(new_socket, hello_terminal, strlen(hello_terminal));
		} 
		else if (strstr(buffer, "GET") != NULL) {
			write(new_socket, hello_browser, strlen(hello_browser));
		}
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
	free(hello_browser);
	free(file_content);
    return 0;
}
