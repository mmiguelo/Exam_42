#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

typedef struct s_client {
	int id;
	char msg[10000];
} t_client;

t_client clients[FD_SETSIZE];
fd_set readfds, writefds, allfds;
int max_fd = 0, nextClientId = 0;
char writeBuffer[12000], readBuffer[12000];

void clean() {
	for (int fd = 0; fd <= max_fd; fd++) {
		if (FD_ISSET(fd, &allfds)) {
			FD_CLR(fd, &allfds);
			close(fd);
		}
	}
}

void fatal(char *msg) {
	clean();
	write(2, msg, strlen(msg));
	exit(1);
}

void broadcast(int senderFd, int serverFd, char *msg) {
	for (int fd = 0; fd <= max_fd; fd++) {
		if (FD_ISSET(fd, &writefds) && fd != senderFd && fd != serverFd)
			send(fd, msg, strlen(msg), 0);
	}
}

int main(int ac, char **av) {
	if (ac != 2) fatal("Wrong number of arguments\n");

	struct sockaddr_in servaddr, cli;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		fatal("Fatal error\n");
	bzero(&servaddr, sizeof(servaddr));
	
	max_fd = sockfd;
	FD_ZERO(&allfds);
	FD_SET(sockfd, &allfds);

	socklen_t addr_len = sizeof(servaddr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));

	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal("Fatal error\n");
	if (listen(sockfd, 10) != 0)
		fatal("Fatal error\n");

	while(1) {
		readfds = writefds = allfds;

		if (select(max_fd + 1, &readfds, &writefds, NULL, NULL) < 0)
			continue;

		for (int fd = 0; fd <= max_fd ; fd++) {
			if (!FD_ISSET(fd, &readfds)) continue;

			if (fd == sockfd) {
				socklen_t len = sizeof(cli);
				int client_fd = accept(sockfd, (struct sockaddr *)&cli, &len);
				if (client_fd < 0) continue;

				max_fd = (client_fd > max_fd) ? client_fd : max_fd;
				clients[client_fd].id = nextClientId++;
				clients[client_fd].msg[0] = '\0';
				FD_SET(client_fd, &allfds);
				sprintf(writeBuffer, "server: client %d just arrived\n", clients[client_fd].id);
				broadcast(client_fd, sockfd, writeBuffer);
			}
			else {
				int n = recv(fd, readBuffer, 10000, 0);

				if (n <= 0) {
					sprintf(writeBuffer, "server: client %d just left\n", clients[fd].id);
					broadcast(fd, sockfd, writeBuffer);
					FD_CLR(fd, &allfds);
					close(fd);
				}
				else {
					int j = strlen(clients[fd].msg);
					for (int i = 0; i < n; i++) {
						clients[fd].msg[j++] = readBuffer[i];
						if (clients[fd].msg[j - 1] == '\n') {
							clients[fd].msg[j - 1] = '\0';
							sprintf(writeBuffer, "client %d: %s\n", clients[fd].id, clients[fd].msg);
							broadcast(fd, sockfd, writeBuffer);
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
