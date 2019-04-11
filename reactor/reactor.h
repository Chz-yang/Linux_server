#ifndef REACTOR_H
#define REACTOR_H

#include <pthread.h>
#include <sys/epoll.h>

class Reactor {
public:
    Reactor(int thread_num = 0, void* (*event_handler)(void*) = nullptr);
    void registerEvent(int fd, bool is_listenfd = false);
    void handlerEvents();
    ~Reactor();

public:
    static void registerEvent(int fd, Reactor *this_ptr);

private:
    void initChildThreads();
    void dispatch(const epoll_event& event);
    static int setnonblocking(int fd);

private:
    int epollfd_;
    int listenfd_;
    int thread_num_;
    pthread_t* child_threads_;
    int** pipefd_;
    int cur_child_index_;
    void* (*event_handler_)(void*);
};

#endif