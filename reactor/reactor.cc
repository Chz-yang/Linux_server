#include "reactor.h"
#include "worker_thread.h"

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

const int kMaxThreadNumber = 10;
const int kMaxEventNumber = 1024;

Reactor::Reactor(int thread_num, void* (*event_handler)(void*)) {
    if (thread_num == 0) {thread_num = kMaxThreadNumber;}

    if (thread_num > kMaxThreadNumber) {
        throw "Thread number is overmuch!";
    } else {
        epollfd_ = epoll_create(5);
        thread_num_ = thread_num;
        cur_child_index_ = 0;
        event_handler_ = event_handler;
        initChildThreads();
    }

    // std::cout << this << std::endl;
}

Reactor::~Reactor() {
    for (int i = 0; i < thread_num_; ++i) {
        pthread_cancel(child_threads_[i]);
        delete []pipefd_[i];
    }

    delete []child_threads_;
    delete []pipefd_;
}

void Reactor::initChildThreads() {
    static std::pair<int, Reactor*> argv[kMaxThreadNumber];

    child_threads_ = new pthread_t[thread_num_];
    pipefd_ = new int*[thread_num_];

    for (int i = 0; i < thread_num_; ++i) {
        pipefd_[i] = new int[2];
        if (pipe(pipefd_[i]) == -1) {
            std::cout << "pipe failure!" << std::endl;
            exit(EXIT_FAILURE);
        }

        argv[i].first = pipefd_[i][0];
        argv[i].second = this;

        pthread_create(
                       &child_threads_[i], 
                       NULL, 
                       event_handler_, 
                       (void*)&argv[i]);
    }
}

void Reactor::registerEvent(int fd, bool is_listenfd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);

    if (is_listenfd) {
        listenfd_ = fd;
        std::string buff = std::to_string(fd);
        for (int i = 0; i < thread_num_; ++i) {
            write(pipefd_[i][1], buff.c_str(), buff.length());
        }
    }

    setnonblocking(fd);
}

void Reactor::dispatch(const epoll_event& event) {
    // std::cout << "dispath to " << cur_child_index_ << std::endl;

    std::string buff = std::to_string(event.data.fd);
    write(pipefd_[cur_child_index_][1], buff.c_str(), buff.length());

    cur_child_index_ = (cur_child_index_ + 1) % thread_num_;
}

int Reactor::setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void Reactor::handlerEvents() {
    epoll_event events[kMaxEventNumber];
    registerEvent(listenfd_);

    while(true){
        int ret = epoll_wait(epollfd_, events, kMaxEventNumber, -1);
        if (ret < 0) {
            std::cout << "epoll failure" << std::endl;
            break;
        }

        for (int i = 0; i < ret; ++i) {
            dispatch(events[i]);
        }
    }
}

void Reactor::registerEvent(int fd, Reactor *this_ptr) {
    this_ptr->registerEvent(fd, false);
}