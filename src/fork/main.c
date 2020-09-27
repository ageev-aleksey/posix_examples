#include <sys/types.h>
#include <unistd.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


char * str_concat(const char *a, const char *b) {
    size_t la = strlen(a);
    size_t lb = strlen(b);
    char *buffer = malloc(la+lb);
    if (buffer == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < la; i++) {
        buffer[i] = a[i];
    }
    for (size_t i = 0; i < lb; ++i) {
        buffer[i + la] = b[i];
    }
    return buffer;
}

void str_clear(char *str) {
    for (size_t i = 0; str[i] != '\0'; ++i) {
        str[i] = '\0';
    }
}


int main() {
    log_info("Start app.");
    pid_t  child = fork();
    if(child == 0) {
        return 1;
    }
    char value[256] = {0};
    sprintf(value, "%d", child);
    char *msg = str_concat("Child process started with pid -> ", value);
    log_info(msg);
    free(msg);
    int cs = 0;
    wait(&cs);
    str_clear(value);
    sprintf(value, "%d", cs);
    msg = str_concat("Child process stop with exit status -> ", value);
    log_info(msg);
    return 0;
}
