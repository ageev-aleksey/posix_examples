#include <sys/socket.h>
#include <sys/types.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Usage: %s -ip <id address> -port", argv[0]);
        return 0;
    }
    int parameter = 0;
    int optindex = 0;
    char ip[11] = {0};
    char port[5] = {0};
    long count = LONG_MAX;
    struct option opts[] = {
            {"ip", true, 0, 'i'},
            {"port", true, 0, 'p'},
            {"c", true, 0, 'c'},
            {0, 0, 0},
    };

    while ((parameter = getopt_long(argc, argv, "i:p:c:", opts, &optindex)) != -1) {
        switch (parameter) {
            case 'i':
                strcpy(ip, optarg);
                break;
            case 'p':
                strcpy(port, optarg);
                break;
            case 'c':
                count = strtol(optarg, NULL, 10);
                break;
            default:
                printf("Invalid value\n");
        }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock) {
        perror("create socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(strtol(port, NULL, 10));
    inet_aton(ip, &addr.sin_addr);
    int res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == res) {
        perror("connection");
        return -1;
    }
    srand(time(NULL));
    char buffer[500] = {0};
    for (long i = 0; i < count; i++) {
        sleep(1);
        sprintf(buffer, "ping -> %d", rand()%100);
        printf("Send to server: %s\n", buffer);
        int res = send(sock, buffer, strlen(buffer), 0);
        if (-1 == res) {
            perror("error sending");
            return -1;
        }
        memset(buffer, 0, sizeof(buffer));
        res = recv(sock, buffer, sizeof(buffer), 0);
        if (-1 == res) {
            perror("error recv");
            return -1;
        }
        printf("Received from server: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    send(sock, "QUIT", 5, 0);
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return 0;
}



