#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "base_worker_thread.h"

template <class T>
struct Argv {
    int listenfd;
    T* this;
};

class WorkerThread : public BaseWorkerThread {
public:
    WorkerThread();
    ~WorkerThread();
public:
    static void work(void *argv);
private:
    void write();
    void read();
    void newConnect();
private:
    int listenfd_;
};

#endif