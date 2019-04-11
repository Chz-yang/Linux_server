#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "reactor.h"

class EventHandler {
public:
    EventHandler() {}
    ~EventHandler() {}
public:
    static void* handlerEvent(void *argv);
private:
    static void connectRequest(int listenfd, Reactor* reactor);
    static void sendRequest(int sockfd, Reactor *reactor);
private:
};

#endif