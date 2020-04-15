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
#include <errno.h>

#define PORT 8080

//return errno
//return EINVAL
//return exit(EXIT_FAILURE)

//perror("")

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
	long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
 
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

	char *file, *file_content, c;
	char *content;
	int file_size, fd, i, j, not_exist = 0;
	char *packet_temp, *packet;
	int len;
	i = 6;
    while(i--) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[2048] = {0};
        valread = read(new_socket, buffer, 2048);
        printf("%s\n", buffer);

		if (strstr(buffer, "info.txt") != NULL) {
			file = "info.txt";
			packet_temp = "HTTP/1.1 200 OK\nContent-type: text/plain\nContent-length: ";
		}
		else if (strstr(buffer, "info.mp3") != NULL) {
			file = "info.mp3";
			packet_temp = "HTTP/1.1 200 OK\nContent-type: audio/mp3\nContent-length: ";
		}
		else if (strstr(buffer, "info.html") != NULL) {
			file = "info.html";
			packet_temp = "HTTP/1.1 200 OK\nContent-type: text/html\nContent-length: ";
		}
		else if (strstr(buffer, "info.mp4") != NULL) {
			file = "info.mp4";
			packet_temp = "HTTP/1.1 200 OK\nContent-type: video/mp4\nContent-encoding: gzip, deflate\nContent-length: ";
		}
		else if (strstr(buffer, "info.jpeg") != NULL) {
			file = "info.jpeg";
			packet_temp = "HTTP/1.1 200 OK\nContent-type: image/png\nContent-length: ";
		}
		else {
			not_exist = 1;
			packet_temp = "HTTP/1.1 200 OK\nContent-type: text/plain\nContent-length: ";
		}

		if (not_exist) {
			content = "ERROR: Requested file does not exist";
			file_size = strlen(content);
			j = 0;
			file_content = (char *)malloc(sizeof(char) * (file_size + 1));
			file_content[file_size] = '\0';
			while (j < file_size) {
				file_content[j] = content[j];
				j++;
			}
		}
		else {
			fd = open(file, O_RDONLY);
			file_size = lseek(fd, 0, SEEK_END);
			lseek(fd, 0, 0);
			file_content = (char *)malloc(sizeof(char) * (file_size + 1));
			file_content[file_size] = '\0';
			j = 0;
			while (j < file_size) {
				read(fd, &c, sizeof(char));
				file_content[j] = c;
				j++;
			}
			close(fd);
		}

		len = strlen(packet_temp);
		packet = (char *)malloc(sizeof(char) * (len + 2 + file_size + 1));
		packet[len] = '\0';
		j = 0;
		while (j < len) {
			packet[j] = packet_temp[j];
			j++;
		}
		len = 0;
		int digit;
		char *d = (char *)malloc(sizeof(char) * 2);
		d[1] = '\0';
		int s = file_size;
		do {
			s /= 10;
			len++;
		} while (s);
		len -= 1;
		while (len >= 0) {
			digit = (int)(file_size / pow(10, len)) % 10;
			d[0] = '0' + digit;
			strcat(packet, d);
			len -= 1;
		}
		free(d);
		strcat(packet, "\n\n");
		strcat(packet, file_content);

		write(new_socket, packet, strlen(packet));
        printf("------------------Content of file sent-------------------\n");
        close(new_socket);
    }
	free(packet);
	free(file_content);
    return 0;
}
