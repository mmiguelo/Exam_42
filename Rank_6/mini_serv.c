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
} t_client;

t_client clients[2048];
fd_set readfds, writefds, activefds;
int max_fd = 0, nextClientId = 0;
char readBuffer[120000], writeBuffer[120000];

void fatal(char *msg) {
    write(2, msg, strlen(msg));
    exit(1);
}

void broadcast(int senderFd, char* msg) {
    for (int fd = 0; fd <= max_fd; fd++) {
        if (FD_ISSET(fd, &writefds) && fd != senderFd)
            send(fd, msg, strlen(msg), 0);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) fatal("Wrong number of arguments\n");

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) fatal("Fatal error\n");

    max_fd = serverSocket;
    FD_ZERO(&activefds);
    FD_SET(serverSocket, &activefds);

    struct sockaddr_in servaddr;
    socklen_t addr_len = sizeof(servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433);
    servaddr.sin_port = htons(atoi(argv[1]));

    if (bind(serverSocket, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 || listen(serverSocket, 128) != 0)
        fatal("Fatal error\n");

    while(1) {
        readfds = writefds = activefds;

        if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0) continue;

        for (int fd = 0; fd <= max_fd; fd++) {
            if (!FD_ISSET(fd, &readfds)) continue;

            if (fd == serverSocket) {
                int clientSocket = accept(serverSocket, (struct sockaddr *)&servaddr, &addr_len);
                if (clientSocket < 0) continue;

                max_fd = (clientSocket > max_fd) ? clientSocket : max_fd;
                clients[clientSocket].id = nextClientId++;
                clients[clientSocket].msg[0] = '\0';
                FD_SET(clientSocket, &activefds);
                sprintf(writeBuffer, "server: client %d just arrived\n", clients[clientSocket].id);
                broadcast(clientSocket, writeBuffer);
            } 
            else {
                int n = recv(fd, readBuffer, sizeof(readBuffer), 0);

                if (n <= 0) { // client disconnected
                    sprintf(writeBuffer, "server: client %d just left\n", clients[fd].id);
                    broadcast(fd, writeBuffer);
                    FD_CLR(fd, &activefds);
                    close(fd);
                } 
                
                else {
					int j = strlen(clients[fd].msg);
					for (int i = 0; i < n; i++) {
						clients[fd].msg[j++] = readBuffer[i];
						if (clients[fd].msg[j - 1] == '\n') {
							clients[fd].msg[j - 1] = '\0';
							sprintf(writeBuffer, "client %d: %s\n", clients[fd].id, clients[fd].msg);
							broadcast(fd, writeBuffer);
							j = 0;
							clients[fd].msg[0] = '\0';
						}
					}
				}
            }
        }
    }
    return 0;
}
