#include "worker_thread.h"
#include "reactor.h"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include <iostream>

typedef std::pair<int, Reactor*> Argv;

void* WorkerThread::work(void *argv) {
    int pipefd = ((Argv*)argv)->first;
    Reactor* reactor = ((Argv*)argv)->second;
    int listenfd;

    char buff[256];
    int ret = ::read(pipefd, buff, 256);
    listenfd = atoi(buff);

    while(true) {
        // std::cout << "listenfd = " << listenfd << " "
        //             << "pipefd = " << pipefd << " " << std::endl;
        ret = ::read(pipefd, buff, 256);

        // std::cout << "new fd = " << buff << std::endl;

        if (ret < 0) {
            std::cout << "Read failure!" << std::endl;
            break;

        } else if (ret == 0) {
            std::cout << "Worker thread exit!" << std::endl;
            break;

        } else {
            int fd = atoi(buff);

            if (fd == listenfd) { // new connect
                WorkerThread::addNewConnect(reactor, listenfd);
            } else {
                WorkerThread::read(fd);
            }
        }
    }

    return nullptr;
}

void WorkerThread::addNewConnect(Reactor* reactor, int listenfd) {
    sockaddr addr;
    socklen_t addr_len;
    int fd = ::accept(listenfd, &addr, &addr_len);

    if (fd == -1) {
        std::cout << "Accept new connecting failure" << std::endl;
    } else {
        // std::cout << "Accept new connecting fd: " << fd << std::endl;
        reactor->registerEvent(fd, false);
        // Reactor::registerEvent(fd, reactor);
    }
}

void WorkerThread::read(int sockfd) {
    char buff[256];
    memset(buff, '\0', 256);
    buff[0] = '[';

    time_t now = time(nullptr);
    char* time_str = ctime(&now);
    memcpy(buff + 1, time_str, strlen(time_str));
    buff[strlen(time_str)] = ']';
    buff[strlen(time_str) + 1] = ' ';

    ::read(sockfd, buff + strlen(buff), 256);

    std::cout << "send back " << buff << std::endl;

    ::write(sockfd, buff, strlen(buff));
}