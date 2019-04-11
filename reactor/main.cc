#include "worker_thread.h"
#include "reactor.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cout << "usage: " << argv[0] << " ip_address port_number"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    bind(sock, (sockaddr*)&address, sizeof(address));

    listen(sock, 5);

    Reactor reactor(2, &WorkerThread::work);
    reactor.registerEvent(sock, true);
    reactor.handlerEvents();

    return 0;
}