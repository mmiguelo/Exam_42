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
    if (argc != 2) fatal("Usage: ./server <port>\n");

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
                memset(clients[clientSocket].msg, 0, sizeof(clients[clientSocket].msg));
                FD_SET(clientSocket, &activefds);
                sprintf(writeBuffer, "server: client %d just arrived\n", clients[clientSocket].id);
                broadcast(clientSocket, writeBuffer);
            } 
            else {
                int n = recv(fd, readBuffer, 100000, 0);

                if (n <= 0) { // client disconnected
                    sprintf(writeBuffer, "server: client %d just left\n", clients[fd].id);
                    broadcast(fd, writeBuffer);
                    FD_CLR(fd, &activefds);
                    close(fd);
                } 
                
                else { // received data
                    for (int i = 0, j = strlen(clients[fd].msg); i < n; i++, j++) {
                        clients[fd].msg[j] = readBuffer[i];
                        if (clients[fd].msg[j] == '\n') {
                            clients[fd].msg[j] = '\0';
                            sprintf(writeBuffer, "client %d: %s\n", clients[fd].id, clients[fd].msg);
                            broadcast(fd, writeBuffer);
                            memset(clients[fd].msg, 0, sizeof(clients[fd].msg));
                        }
                    }
                }
            }
        }
    }
    return 0;
}
