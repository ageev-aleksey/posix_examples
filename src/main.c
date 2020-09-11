#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <getopt.h>


typedef struct sockaddr* (*InitReq)(char *, char**);


struct sockaddr* init(void) {
    struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(80);
    return (struct sockaddr *) addr;
}

void request_create(char *hostname, char **buffer) {
    *buffer = malloc(sizeof(char)*1024);
    sprintf(*buffer, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", hostname);
}

struct sockaddr* init_by_ip(char * ip, char **http_request) {
    struct sockaddr_in *addr = (struct sockaddr_in *)  init();
    int error = inet_aton(ip, &addr->sin_addr);
    if(error == 0) {
        return NULL;
    }
    struct hostent* host = gethostbyaddr(addr, sizeof(struct sockaddr_in), AF_INET);
    request_create(host->h_name, http_request);
    return (struct sockaddr *) addr;
}

struct sockaddr* init_by_hostname(char *hostname, char **http_request) {
    struct sockaddr_in *addr = (struct sockaddr_in *)  init();
    struct addrinfo *info;
    int error = getaddrinfo(hostname, NULL, NULL, &info);
    if(error) {
        return NULL;
    }
    addr->sin_addr = ((struct sockaddr_in*)info->ai_addr)->sin_addr;
    char str_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), str_ip, INET_ADDRSTRLEN);
    printf("ip-> %s\n", str_ip);
    request_create(hostname, http_request);
    return (struct sockaddr *) addr;
}


int main(int argc, char **argv) {
    InitReq initialize = NULL;
    char *path = malloc(1024);
   if(argc == 1) {
       printf("-==Simple http get page v 0.1==-\n For info how usage program use argument --help(-h)\n");
       return 0;
   } else {
       int val = 0;
       struct option opts[] = {
               {"help", 0, 0, 'h'},
               {"domen", 1, 0, 'd'},
               {"ip", 1, 0, 'i'},
               {0, 0, 0, 0}
       };
        int optindx = 0;

       while((val = getopt_long(argc, argv, "hd:i:", opts, &optindx)) != -1) {
           switch (val) {
               case 'h':
                   printf("help\n");
                   break;
               case 'd':
                   printf("domen -> %s\n", optarg);
                   initialize = init_by_hostname;
                   strcpy(path, optarg);
                   break;
               case 'i':
                   printf("ip -> %s\n", optarg);
                   initialize = init_by_ip;
                   strcpy(path, optarg);
                   break;
           }
       }
   }


    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("socket");
        return -1;
    }
//    //////GET YANDEX IP///////////////
//    struct addrinfo* result;
//    int error = getaddrinfo("example.com", NULL, NULL, &result);
//    if(error == -1) {
//        perror("getadrrinfo");
//        return -1;
//    }
//    char hostname[1024];
//    error = getnameinfo(result->ai_addr, result->ai_addrlen, hostname, 1024, NULL, 0, 0);
//    printf(hostname);
    //////////////////////////////////
    char *request;
    struct sockaddr_in *addr = initialize(path, &request);
    printf("request: %s", request);
    int res = connect(sock, addr, sizeof(struct sockaddr_in));
    if(res == -1) {
        perror("connect");
        return -1;
    }
    res = 0;
    int tmp = 0;
    while(res < sizeof(request)) {
        tmp = send(sock, request, strlen(request), 0);
        if(tmp == -1) {
            perror("send");
            return -1;
        }
        res += tmp;
    }
    char buffer[10240];
    memset(buffer, 0, sizeof(buffer));
    res = recv(sock, buffer, sizeof(buffer), 0);
    if(res == -1) {
        perror("recv");
        return -1;
    }
    printf(buffer);
    return 0;
}