#include "event_handler.h"
#include "reactor.h"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include <iostream>

extern int kMaxEventNumber;

EventHandler::EventHandler(int pipefd) {
    pipefd_ = pipefd;
    epollfd_ = epoll_create(5);

    addfd(pipefd_);
}

EventHandler::~EventHandler() {
    for (auto sockfd : sockfds_) {
        removefd(sockfd);
        close(sockfd);
    }

    removefd(pipefd_);
    close(pipefd_);
}

void* EventHandler::handlerEvent(void *argv) {
    int read_pipefd = *(int*)argv;

    EventHandler event_handler(read_pipefd);
    event_handler.handlerEvent();

    return nullptr;
}

void EventHandler::handlerEvent() {
    epoll_event events[kMaxEventNumber];
    while(true) {
        int ret = epoll_wait(epollfd_, events, kMaxEventNumber, -1);

        if (ret < 0) {
            std::cout << "Child::epoll failure" << std::endl;
            break;
        }

        for (int i = 0; i < ret; ++i) {
            if (events[i].data.fd == pipefd_) {
                EventHandler::connectRequest();
            } else {
                EventHandler::sendRequest(events[i].data.fd);
            }
        }
    }
}

void EventHandler::connectRequest() {
    static char buff[256];
    memset(buff, '\0', 256);

    int ret = read(pipefd_, buff, 256);
    if (ret < 0) {
        std::cout << "read failure" << std::endl;

    } else if (ret == 0) {
        std::cout << "child thread exit" << std::endl;
        exit(EXIT_SUCCESS);

    } else {
        EventHandler::addfd(atoi(buff));
    }
}

void EventHandler::sendRequest(int sockfd) {
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
        removefd(sockfd);
        return;
    }

    std::cout << "send back " << buff << std::endl;

    ::write(sockfd, buff, strlen(buff));
}

void EventHandler::addfd(int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);

    Reactor::setnonblocking(fd);
    sockfds_.insert(fd);
}

void EventHandler::removefd(int fd) {
    sockfds_.erase(fd);

    epoll_event event;
    event.data.fd = fd;
    epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event);
}