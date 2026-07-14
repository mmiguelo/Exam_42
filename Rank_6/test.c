#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

typedef struct s_client {
    int id;
    char msg[100000];
}   t_client;

t_client clients[2048];
fd_set readfds, writefds, activefds;
int max_fd = 0, nextClientId = 0;
char readBuffer[120000], writeBuffer[120000];

void fatal() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

void broadcast(int senderFd, char* msg) {
    for (int fd = 0; fd <= mas_fd; fd++) {
        if (FD_ISSET(fd, &writefds) && fd != senderFd)
            send(fd, msg, strlen(msg), 0);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) write(2, "Wrong number of arguments\n", 26);

    int sockfd;
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) fatal();

	// assign IP, PORT 
    struct sockaddr_in servaddr;
    socklen_t addr_len = sizeof(servaddr);
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(8081);
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) fatal();

	if (listen(sockfd, 10) != 0) fatal();

    while(1) {
        readfds = writefds = activefds;

        if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) , 0) continue;

        for(int fd = 0; fd <= max_fd; fd++) {
            if (!FD_ISSET(fd, &readfds)) continue;

            if (fd == sockfd) {
                int clientSocket = acccept(sockfd, (struct sockaddr *)&servaddr, &addr_len);

                if (clientSocket < 0) continue;
                
                max_fd = (clientSocket > max_fd) ? clientSocket : max_fd;
                

            }
            else
                printf("server acccept the client...\n");
        }
    }
}