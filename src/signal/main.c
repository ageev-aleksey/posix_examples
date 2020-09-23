#include <signal.h>
#include <string.h>
#include "log.h"
#include <unistd.h>
#include <pthread.h>

void signal_handler(int signal) {
    char b[110];
    memset(b, 0, sizeof(b));
    sprintf(b, "\"Start process signal: %d", signal);
    log_trace(b);
    sleep(5);
    memset(b, 0, sizeof(b));
    sprintf(b, "\"STOP process signal: %d", signal);
    log_trace(b);
}

void* thread_impl(void *args) {
    log_trace("Starting thread");
    sleep(2);
    raise(SIGUSR2);
    return NULL;
}

int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1); // блокировка сигнала при его обработке
    act.sa_mask = set;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    pthread_t thread;
    // pthread_create(&thread, NULL, thread_impl, NULL);
    pid_t pid = fork();
    if (pid) {
        kill(pid, SIGUSR2);
        return 0;
    }
    // sleep(2);
    raise(SIGUSR1);
   // pthread_join(thread, NULL);
    log_trace("EXIT!");
    return 0;
}