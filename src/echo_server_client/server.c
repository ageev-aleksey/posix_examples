#include <sys/socket.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <asm/errno.h>
#include <errno.h>

__sig_atomic_t isRun = true;

void signal_handler(int signal) {
    isRun = false;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: %s --ip <ip of interface> --port <port> \n", argv[0]);
        return -1;
    }

    struct option opts[] = {
            {"ip", true, 0, 'i'},
            {"port", true, 0, 'p'},
            {0, 0, 0},
    };
    int parameter = 0;
    int optindex = 0;
    char ip[11] = {0};
    char port[5] = {0};
    while ((parameter = getopt_long(argc, argv, "i:p:", opts, &optindex)) != -1) {
        switch (parameter) {
            case 'i':
                strcpy(ip, optarg);
                break;
            case 'p':
                strcpy(port, optarg);
                break;
            default:
                printf("Error");
                return -1;
        }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sock) {
        perror("create socket");
        return -1;
    }
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = signal_handler;
    action.sa_flags = SA_ONESHOT;
    sigset_t mask_set;
    sigfillset(&mask_set);
    action.sa_mask = mask_set;
    sigaction(SIGINT, &action, NULL);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(strtol(port, NULL, 10));
    inet_aton(ip, &addr.sin_addr);
    int res = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        return -1;
    }
    listen(sock, 0);
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));

    int client_sock = 0;
    unsigned int i = 0;
    while (isRun) {
        i++;
        socklen_t client_addr_len = 0;
        client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock == -1) {
            if (EINTR == errno) {
                continue;
            }
            perror("Client connecting error");
            return -1;
        }
        char buffer[500];
        memset(buffer, 0, sizeof(buffer));
        srand(time(NULL));
        int size = recv(client_sock, buffer, sizeof(buffer), 0);
//        if (strcmp(buffer, "QUIT") == 0) {
//            printf("%s\n", "Client send request for close session!");
//            break;
//        }
        printf("%du: from client received: %s\n",i,  buffer);
        //  memset(buffer, 0, sizeof(buffer));
        char send_buffer[500] = {0};
        strcpy(send_buffer, buffer);
        size = send(client_sock, send_buffer, strlen(send_buffer), 0);
        if (-1 == size) {
            perror("sending to client error");
            break;
        }
        shutdown(client_sock, SHUT_RDWR);
        close(client_sock);
    }
    shutdown(client_sock, SHUT_RDWR);
    close(client_sock);
    close(sock);
    printf("Good by!\n");
    return 0;
}