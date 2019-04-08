#ifndef BASE_WORKER_THREAD_H
#define BASE_WORKER_THREAD_H

class BaseWorkerThread {
public:
    void virtual run(void **arv) = 0;
    virtual ~BaseWorkerThread() {}
};

#endif