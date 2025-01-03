#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <data_processor.hpp>

static int fd_201 = -1;
static int fd_202 = -1;
static int fd_203 = -1;

static void at_exit()
{
    if (fd_201 != -1) {
        close(fd_201);
        fd_201 = -1;
    }

    if (fd_202 != -1) {
        close(fd_202);
        fd_202 = -1;
    }

    if (fd_203 != -1) {
        close(fd_203);
        fd_203 = -1;
    }
}

int create_socket()
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket(SOCK_DGRAM)");
        exit(1);
    }

    int value = 1;
    int res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, 
        (const void *)&value , sizeof(int));
    if (res < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        exit(1);   
    }

    return fd;
}

void bind_socket(int fd, int port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);

    int res = bind(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (res < 0) {
        perror("bind");
        exit(1);   
    }
}

int main()
{
    atexit(at_exit);

    data_processor dp;

    fd_201 = create_socket();
    fd_202 = create_socket();
    fd_203 = create_socket();

    bind_socket(fd_201, 201);
    bind_socket(fd_202, 202);
    bind_socket(fd_203, 203);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while(1) {
        static char buffer[2048];
        memset(buffer, 0, sizeof(buffer));

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd_201, &fds);
        FD_SET(fd_202, &fds);
        FD_SET(fd_203, &fds);

        int res = select(fd_203 + 1, &fds, NULL, NULL, NULL);

        if (res > 0) {
            if (FD_ISSET(fd_201, &fds)) {
                int len = recvfrom(fd_201, buffer, sizeof(buffer)-1, 0,
		        (struct sockaddr *) &client_addr, &client_len);
                dp.data_ind(buffer, sizeof(buffer));
            }
            
            if (FD_ISSET(fd_202, &fds)) {
                int len = recvfrom(fd_202, buffer, sizeof(buffer)-1, 0,
		        (struct sockaddr *) &client_addr, &client_len);
                dp.data_ind(buffer, sizeof(buffer));
            }

            if (FD_ISSET(fd_203, &fds)) {
                int len = recvfrom(fd_203, buffer, sizeof(buffer)-1, 0,
		        (struct sockaddr *) &client_addr, &client_len);
                dp.data_ind(buffer, sizeof(buffer));
            }
        }
    }

    return 0;
}