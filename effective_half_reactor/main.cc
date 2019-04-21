#include "reactor.h"
#include "event_handler.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
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
    if (sock < 0) {
        std::cout << "Create socket failure" << std::endl;
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    bind(sock, (sockaddr*)&address, sizeof(address));

    if(listen(sock, 5) < 0) {
        std::cout << "Listen socket failure" << std::endl;
        exit(EXIT_FAILURE);
    }

    Reactor reactor(10, &EventHandler::handlerEvent);
    reactor.setListenfd(sock);
    reactor.run();

    close(sock);

    return 0;
}