#ifndef REACTOR_H
#define REACTOR_H

#include <pthread.h>

const int kThreadMaxNum = 10;

template <class T>
class Reactor {
public:
    Reactor(int thread_num = kThreadMaxNum);
    void run();
    ~Reactor();
private:
    int thread_num_;
    pthread_t* child_threads_;
    int cur_child_index_;
};

#endif