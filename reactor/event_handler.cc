#include "event_handler.h"
#include "reactor.h"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include <iostream>

typedef std::pair<int, Reactor*> Argv;

void* EventHandler::handlerEvent(void *argv) {
    int pipefd = ((Argv*)argv)->first;
    Reactor* reactor = ((Argv*)argv)->second;
    int listenfd;

    char buff[256];
    memset(buff, '\0', 256);

    int ret = ::read(pipefd, buff, 256);
    listenfd = atoi(buff);

    while(true) {
        memset(buff, '\0', 256);
        ret = ::read(pipefd, buff, 256);

        if (ret < 0) {
            std::cout << "Read failure!" << std::endl;
            break;

        } else if (ret == 0) {
            std::cout << "Worker thread exit!" << std::endl;
            break;

        } else {
            int fd = atoi(buff);

            if (fd == listenfd) { // new connection
                EventHandler::connectRequest(listenfd, reactor);
            } else {
                EventHandler::sendRequest(fd, reactor);
            }
        }
    }

    return nullptr;
}

void EventHandler::connectRequest(int listenfd, Reactor* reactor) {
    sockaddr addr;
    socklen_t addr_len;
    int fd = ::accept(listenfd, &addr, &addr_len);

    if (fd == -1) {
        std::cout << "Accept new connection failure" << std::endl;
    } else {
        reactor->registerEvent(fd, false);
    }
}

void EventHandler::sendRequest(int sockfd, Reactor *reactor) {
    static char buff[256];
    memset(buff, '\0', 256);
    buff[0] = '[';

    time_t now = time(nullptr);
    char* time_str = ctime(&now);
    memcpy(buff + 1, time_str, strlen(time_str));
    buff[strlen(time_str)] = ']';
    buff[strlen(time_str) + 1] = ' ';

    int ret = ::read(sockfd, buff + strlen(buff), 256);
    if (ret == 0) { // the sockfd is closed remotely
        reactor->removeEvent(sockfd);
        return;
    }

    std::cout << "send back " << buff << std::endl;

    ::write(sockfd, buff, strlen(buff));
}