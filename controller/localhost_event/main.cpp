#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {

    if (argc <2) {
        printf("usage: localhost_event <event_id>\n");
        printf("\n");
        printf("Supported events:\n");
        printf(" - state_change_upper_led\n");
        printf(" - state_change_lower_led\n");
        printf(" - state_change_ventilation_fan\n");
        printf(" - state_change_wind_sim_fan\n");
        printf(" - state_change_cooling_rod\n");
        printf(" - state_change_water_circulation\n");
        printf(" - state_change_o2_electrolysis\n");
        printf(" - full_spectrum_led_on\n");
        printf(" - full_spectrum_led_off\n");

        return 1;
    }

	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (strstr(argv[1],"state_change") != NULL) {
	    server_addr.sin_port = htons(203);
    } else if (strstr(argv[1],"full_spectrum_led") != NULL) {
        server_addr.sin_port = htons(301);
    }

	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

    char message[1024];
    snprintf(message, sizeof(message),"localhost_event '%s'", argv[1]);

    int len = sendto(fd, message, strlen(message), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));

	close(fd);

	return 0;
}
