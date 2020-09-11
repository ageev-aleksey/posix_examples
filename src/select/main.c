#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define SELECT_TIMEOUT 0
#define SELECT_ERROR -1

static const int STDOUT = 0;
static const int STDIN = 1;


void function(int i) {
    char *ptr = NULL;
    if (i == 0) {
        goto fexit;
    } else if (i > 5) {
        ptr = malloc(i);
        goto fexit;
    }

fexit:
    if(ptr != NULL) {
        free(ptr);
    }
}



int main(int argc, char **argv) {
    fd_set patternSet;
    struct timeval patternTime;
    FD_ZERO(&patternSet);
    FD_SET(STDIN, &patternSet);
    patternTime.tv_sec = 2;
    patternTime.tv_usec = 0;

    char buffer[256] = {'\0'};
    while (true) {
        fd_set workSet = patternSet;
        struct timeval time = patternTime;
        int res = select(FD_SETSIZE, &workSet, NULL, NULL, &time);
        if (FD_ISSET(STDIN, &workSet)) {
            write(STDOUT, "OK!", 4);
        }
        if (res == SELECT_TIMEOUT) {
            const char v[] = ": select timeout \n";
             write(STDOUT, v, sizeof(v));
        } else if (res == SELECT_ERROR) {
            const char v[] = ": select ERROR -> EXIT \n";
            write(STDOUT, (void*)v, sizeof(v));
            break;
        } else {
            read(STDIN, buffer, 256);
            write(STDOUT, buffer, strlen(buffer));
            for(int i = 0; i < 256 && buffer[i] != '\0'; i++) {
                buffer[1] = '\0';
            }
        }
    }
}