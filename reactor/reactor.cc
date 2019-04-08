#include "reactor.h"

#include <iostream>

template <class T>
Reactor<T>::Reactor(int thread_num) {
    if (thread_num > kThreadMaxNum) {
        std::cout << "Thread number is overmuch!"
                  << std::endl;
        throw ;
    } else {
        thread_num_ = thread_num;
        child_threads_ = new pthread_t[thread_num];
        cur_child_index_ = 0;
    }
}

template <class T>
Reactor<T>::~Reactor() {
    delete []child_threads_;
    delete []children_;
}