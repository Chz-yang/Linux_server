#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;

void* fun(void *argv) {
    int pipefd = *(int*)argv;
    char buff[256];
    int ret = 1;
    // while (ret != 0) {
    ret = read(pipefd, buff, 256);
    cout << "child thread: " << buff << endl;
    // }
    cout << "child thread exit" << endl;

    return NULL;
}

int main() {
    int pipefd[2];
    pipe(pipefd);
    pthread_t pid[2];
    pthread_create(&pid[0], NULL, fun, (void*)&pipefd[0]);

    char buff[256] = "123";
    // memset(buff, 0, 256);
    // write(pipefd[1], buff, 256);
    pthread_create(&pid[1], NULL, fun, (void*)&pipefd[0]);
    // write(pipefd[1], buff, 256);

    pthread_join(pid[0], NULL);
    pthread_join(pid[1], NULL);
}