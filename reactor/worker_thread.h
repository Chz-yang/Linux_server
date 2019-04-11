#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "base_worker_thread.h"
#include "reactor.h"

class WorkerThread : public BaseWorkerThread {
public:
    WorkerThread() {}
    ~WorkerThread() {}
public:
    static void* work(void *argv);
private:
    static void read(int sockfd);
    static void addNewConnect(Reactor* reactor, int listenfd);
private:
};

#endif