#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <chrono>
#include <string.h>
#include <unistd.h>

static int fd_301 = -1;

static void at_exit()
{
    if (fd_301 != -1) {
        close(fd_301);
        fd_301 = -1;
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

enum class action
{
    unknown,
    full_spectrum_led_on,
    full_spectrum_led_off,
};

void handle_action(action a)
{
    /******* STOP ***********/
    // Always allow stop
    if (a == action::full_spectrum_led_off) {
        system("/usr/local/bin/rc_ctrl_16 -c \"relay_04 off\" --server-ip=192.168.202.250");
        return;
    }

    /******* START ***********/
    if (a == action::full_spectrum_led_on) {
        // Start depends on time of day
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *localtime(&now_time_t);
        
        int hour = now_tm.tm_hour;

        if (hour >= 22 && hour <= 5) {
            system("/usr/local/bin/rc_ctrl_16 -c \"relay_04 on\" --server-ip=192.168.202.250");
            return;
        }
    }
}

void handle_sock_buffer(const char* buffer, const int buffer_len)
{
    // Prevent sensor hysteresis
    static time_t ref_ts;
    static bool first_run = true;

    if (first_run) {
        memset(&ref_ts, 0, sizeof(ref_ts));
        first_run = false;
        
        // Pass through for this rounds
    } else {
        auto now = time(nullptr);
        if (ref_ts == now) {
            printf("Ignore repeated command\n");
            return;
        }
    }

    ref_ts = time(nullptr);

    std::vector<char> str;
    for(int i=0; i<buffer_len;i++) {
        // Filter: Exclude control characters
        if (buffer[i] >= 32) {
            str.emplace_back(buffer[i]);
        }
    }

    auto command = std::string(str.data());

    if (command.find("full_spectrum_led_on")) {
        handle_action(action::full_spectrum_led_on);
    }

    if (command.find("full_spectrum_led_off")) {
        handle_action(action::full_spectrum_led_off);
    }
}

int main()
{
    atexit(at_exit);

    fd_301 = create_socket();

    bind_socket(fd_301, 301);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while(1) {
        static char buffer[2048];
        memset(buffer, 0, sizeof(buffer));

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd_301, &fds);

        int res = select(fd_301 + 1, &fds, NULL, NULL, NULL);

        if (res > 0) {
            if (FD_ISSET(fd_301, &fds)) {
                int len = recvfrom(fd_301, buffer, sizeof(buffer)-1, 0,
		        (struct sockaddr *) &client_addr, &client_len);
                handle_sock_buffer(buffer, sizeof(buffer));
            }
        }
    }

    return 0;
}
