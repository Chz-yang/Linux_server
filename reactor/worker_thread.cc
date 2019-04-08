#include "worker_thread.h"

WorkerThread::WorkerThread() {}

void WorkerThread::work(void *argv) {
    Argv<WorkerThread>* arg = static_cast<Argv<WorkerThread>*>(argv);
    int listenfd = arg->listenfd;
    
}
