#ifndef REACTOR_H
#define REACTOR_H

#include <set>
#include <pthread.h>
#include <sys/epoll.h>
#include <iostream>

class Reactor {
public:
    Reactor(int thread_num = 0, void* (*event_handler)(void*) = nullptr);
    void setListenfd(int listenfd);
    void run();
    ~Reactor();

public:
    static int setnonblocking(int fd);

private:
    void initChildThreads();
    void dispatch(int sockfd);

private:
    int epollfd_;
    int listenfd_;
    int thread_num_;
    pthread_t* child_threads_;
    int** pipefd_;
    int cur_child_index_;
    void* (*event_handler_)(void*);
    std::set<int> sockfds_;
};

#endif