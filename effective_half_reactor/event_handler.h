#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <set>

class EventHandler {
public:
    static void* handlerEvent(void *argv);

private:
    EventHandler(int pipefd);
    ~EventHandler();
    void handlerEvent();
    void addfd(int fd);
    void removefd(int fd);
    void connectRequest();
    void sendRequest(int sockfd);

private:
    int pipefd_;
    int epollfd_;
    std::set<int> sockfds_;
};

#endif