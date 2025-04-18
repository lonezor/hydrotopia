#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <rc/relay_16.h>

#define PROGRAM_VERSION "relay_ctrl v0.5"

//-------------------------------------------------------------------------------------------------------------------

enum cli_option
{
    cli_option_list_commands = 1000, // not colliding with ASCII range
    cli_option_relay_command,
    cli_option_server_ip,
    cli_option_help,
    cli_option_version,
};

//-------------------------------------------------------------------------------------------------------------------

static bool g_help = false;
static bool g_version = false;
static bool g_list_commands = false;
static rc_relay_channel_t g_relay_channel = rc_relay_channel_none;
static bool g_relay_set = true;
static bool g_relay_enabled = false;
static const char* g_server_ip = NULL;

static struct option long_options[] = {
    { "list-commands", no_argument, NULL, cli_option_list_commands },
    { "command", required_argument, NULL, cli_option_relay_command },
    { "server-ip", required_argument, NULL, cli_option_server_ip},
    { "help", no_argument, NULL, cli_option_help },
    { "version", no_argument, NULL, cli_option_version },
    { NULL, 0, NULL, 0 }
};

//-------------------------------------------------------------------------------------------------------------------

static void print_commands()
{
    printf("Available commands:\n");
    printf(" - relay_all <on/off>\n");
    printf(" - relay_01  <on/off>\n");
    printf(" - relay_02  <on/off>\n");
    printf(" - relay_03  <on/off>\n");
    printf(" - relay_04  <on/off>\n");
    printf(" - relay_05  <on/off>\n");
    printf(" - relay_06  <on/off>\n");
    printf(" - relay_07  <on/off>\n");
    printf(" - relay_08  <on/off>\n");
    printf(" - relay_09  <on/off>\n");
    printf(" - relay_10  <on/off>\n");
    printf(" - relay_11  <on/off>\n");
    printf(" - relay_12  <on/off>\n");
    printf(" - relay_13  <on/off>\n");
    printf(" - relay_14  <on/off>\n");
    printf(" - relay_15  <on/off>\n");
    printf(" - relay_16  <on/off>\n");
    printf("\n");
}

//-------------------------------------------------------------------------------------------------------------------

static void print_help()
{
    // clang-format off
    printf("Usage: relay_ctrl [OPTIONS]\n");
    printf("\n");
    printf("Options (in order of importance):\n");
    printf(" -l --list-commands     List available commands\n");
    printf(" -c --command=STRING    Run command\n");
    printf("    --server-ip=STRING  Control remote relay over IP network (UDP port 20)\n");
    printf(" -h --help              This help screen\n");
    printf("    --version           Prints version and exits\n");
    printf("\n");
    // clang-format on
}

//-------------------------------------------------------------------------------------------------------------------

static void print_version()
{
    printf("%s\n", PROGRAM_VERSION);
}

//-------------------------------------------------------------------------------------------------------------------

static bool parse_arguments(int argc, char* argv[])
{
    // Parse command line arguments
    int c;
    int option_index = 0;
    while (true) {
        // Using getopt_long API - NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        c = getopt_long(argc, argv, "lc:h", long_options, &option_index);

        // All options parsed
        if (c == -1) {
            break;
        }

        switch (c) // Listed in order of importance
        {
            case 'l':
            case cli_option_list_commands:
                g_list_commands = true;
                break;
            case 'c':
            case cli_option_relay_command: {
                // Get command context
                if (strstr(optarg, "query")) {
                    g_relay_set = false;
                } else { // set
                    g_relay_enabled = strstr(optarg, "on") ? true : false;
                }

                // Get channel (it is small enough, so no need to parse number)
                if (strstr(optarg, "relay_all")) {
                    g_relay_channel = rc_relay_channel_all;
                } else if (strstr(optarg, "relay_01")) {
                    g_relay_channel = rc_relay_channel_01;
                } else if (strstr(optarg, "relay_02")) {
                    g_relay_channel = rc_relay_channel_02;
                } else if (strstr(optarg, "relay_03")) {
                    g_relay_channel = rc_relay_channel_03;
                } else if (strstr(optarg, "relay_04")) {
                    g_relay_channel = rc_relay_channel_04;
                } else if (strstr(optarg, "relay_05")) {
                    g_relay_channel = rc_relay_channel_05;
                } else if (strstr(optarg, "relay_06")) {
                    g_relay_channel = rc_relay_channel_06;
                } else if (strstr(optarg, "relay_07")) {
                    g_relay_channel = rc_relay_channel_07;
                } else if (strstr(optarg, "relay_08")) {
                    g_relay_channel = rc_relay_channel_08;
                } else if (strstr(optarg, "relay_09")) {
                    g_relay_channel = rc_relay_channel_09;
                } else if (strstr(optarg, "relay_10")) {
                    g_relay_channel = rc_relay_channel_10;
                } else if (strstr(optarg, "relay_11")) {
                    g_relay_channel = rc_relay_channel_11;
                } else if (strstr(optarg, "relay_12")) {
                    g_relay_channel = rc_relay_channel_12;
                } else if (strstr(optarg, "relay_13")) {
                    g_relay_channel = rc_relay_channel_13;
                } else if (strstr(optarg, "relay_14")) {
                    g_relay_channel = rc_relay_channel_14;
                } else if (strstr(optarg, "relay_15")) {
                    g_relay_channel = rc_relay_channel_15;
                } else if (strstr(optarg, "relay_16")) {
                    g_relay_channel = rc_relay_channel_16;
                }
                break;
            }
            case cli_option_server_ip:
                g_server_ip = optarg;
                break;
            case cli_option_help:
                g_help = true;
                break;
            case cli_option_version:
                g_version = true;
                break;
            default:
                continue;
        }
    }

    if (g_help || g_version || g_list_commands) {
        return true;
    }

    if (g_relay_channel == rc_relay_channel_none) {
        fprintf(stderr, "error: No command given! (--help for more information)\n");
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------

static void send_command_to_server()
{
    struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(20);
	server_addr.sin_addr.s_addr = inet_addr(g_server_ip);

	int fd;
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return;
	}

    relay_cmd_msg_t msg;
    memset(&msg, 0, sizeof(msg));
    msg.magic = htonl((uint32_t)RELAY_MSG_MAGIC);
    msg.port = (uint8_t)0;
    msg.nr_channels = (uint8_t)16;
    msg.channel = htons((uint16_t)g_relay_channel);
    msg.state = (uint8_t)g_relay_enabled;

    int res = sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (res != -1) {
        printf("Command sent successfully\n");
    }

	close(fd);
}

int main(int argc, char* argv[])
{
    // Parse mandatory and optional arguments
    if (!parse_arguments(argc, argv)) {
        return EXIT_FAILURE;
    }

    // Print Help Screen and exit
    if (g_help) {
        print_help();
        return EXIT_SUCCESS;
    }

    // Print Version Screen and exit
    if (g_version) {
        print_version();
        return EXIT_SUCCESS;
    }

    if (g_list_commands) {
        print_commands();
        return EXIT_SUCCESS;
    }

    if (g_relay_set) {
        if (g_server_ip != NULL) {
            send_command_to_server();
        } else {
            rc_relay_channel_init();
            rc_relay_channel_set(g_relay_channel, g_relay_enabled);
        }
    } else { // query
        if (g_server_ip != NULL) {
            printf("query mode not yet implemented\n");
        } else {
            bool enabled = rc_relay_channel_get(g_relay_channel);
            printf("value: %s\n", enabled ? "true" : "false");
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------

