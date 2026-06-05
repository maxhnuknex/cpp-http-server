#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include<string>
#include <functional>

#include "ThreadPool.h"

class TCPserver
{
    int port;
    int server_fd;

    int setupServer();
    void serverClient(int,std::function<std::string(const std::string&)> requestHandler);


    int createServerSocket();
    int bindServerSocket();
    int listenForClients();

    int acceptClient();
    std::string recvClient(int client_fd);

public:
    TCPserver (int port);

    void start(std::function<std::string(const std::string&)> hendl);
};