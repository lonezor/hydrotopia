#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#ifdef RELAY_MODULE_16_CHANNELS
#include <rc/relay_16.h>
#endif // RELAY_MODULE_16_CHANNELS

#ifdef RELAY_MODULE_32_CHANNELS
#include <rc/relay_32.h>
#endif // RELAY_MODULE_32_CHANNELS

static int fd_20 = -1;

static void at_exit()
{
    if (fd_20 != -1) {
        close(fd_20);
        fd_20 = -1;
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

static void bind_socket(int fd, int port)
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

static void relay_cmd(char* buffer, int buff_len)
{
    // Safety
    if (buff_len < sizeof(relay_cmd_msg_t)) {
        return;
    }

    relay_cmd_msg_t* msg = (relay_cmd_msg_t*)buffer;

    msg->magic = ntohl(msg->magic);
    msg->channel = ntohs(msg->channel);

    if (msg->magic != RELAY_MSG_MAGIC) {
        return;
    }

    if (msg->nr_channels == 16) {
#ifdef RELAY_MODULE_16_CHANNELS
        printf("RX - 16 channel relay: channel: %08x, relay_state: %d\n", (int)msg->channel, (int)msg->state);

        rc_relay_channel_init();
        rc_relay_channel_set((rc_relay_channel_t)msg->channel, (bool)msg->state);
#endif // RELAY_MODULE_16_CHANNELS
    } else if (msg->nr_channels == 32) {
#ifdef RELAY_MODULE_32_CHANNELS
    printf("RX - 32 channel relay: port: %d, channel: %08x, relay_state: %d\n", 
            (int)msg->port, (int)msg->channel, (int)msg->state);

    rc_relay_channel_init();
    rc_relay_channel_set((rc_relay_port_t)msg->port, (rc_relay_channel_t)msg->channel, (bool)msg->state);
#endif // RELAY_MODULE_32_CHANNELS
    }
}

int main()
{
    atexit(at_exit);

    fd_20 = create_socket();

    bind_socket(fd_20, 20);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while(1) {
        static char buffer[2048];
        memset(buffer, 0, sizeof(buffer));

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd_20, &fds);

        int res = select(fd_20 + 1, &fds, NULL, NULL, NULL);

        if (res > 0) {
            if (FD_ISSET(fd_20, &fds)) {
                int len = recvfrom(fd_20, buffer, sizeof(buffer)-1, 0,
		        (struct sockaddr *) &client_addr, &client_len);
                relay_cmd(buffer, sizeof(buffer));
            }
        }
    }

    return 0;
}
