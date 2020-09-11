#include <log.h>
#include <poll.h>
#include <sys/time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define POLL_TIMEOUT 0
static const int STDOUT = 0;
static const int STDIN = 1;


int main() {
    struct pollfd fd;
    struct timeval time;
    fd.fd = STDIN;
    fd.events = POLLIN;
    fd.revents = 0;
    time.tv_sec = 2;
    time.tv_usec = 0;
    char buffer[256] = {0};
    while (true) {
        struct pollfd wfd = fd;
        struct timeval wtime = time;
        int res = poll(&wfd, 1, 2000); // timeout 2 sec
        if (res == POLL_TIMEOUT) {
            log_info("poll timeout");
        } else if(res < 0) {
            log_warn("poll error");
        } else {
            read(STDIN, buffer, sizeof(buffer));
            size_t l = strlen(buffer);
            if(strcmp(buffer, "\\q") == 0) {
                break;
            }
            write(STDOUT, buffer, l);
            for(int i = 0; i <=  l; i++) {
                buffer[i] == '\0';
            }
        }
    }
}