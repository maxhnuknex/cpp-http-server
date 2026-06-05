#pragma once
#include "../../include/TCPserver/tcpserver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <string>

TCPserver::TCPserver(int port) : port(port), server_fd(-1)
{}

void TCPserver::start(std::function<std::string(const std::string&)> requestHandler)
{
    ThreadPool Trpool(4);

    if (setupServer() == -1)
    {
        return;
    }

    while (true)
    {
        int client_fd = acceptClient();
        if (client_fd == -1)continue;

        Trpool.addTask([&]{
            serverClient(client_fd, requestHandler);
        });
    }
}

int TCPserver::setupServer()
{
    if (createServerSocket() == -1)
    {
        return -1;
    }

    if (bindServerSocket() == -1)
    {
        return -1;
    }

    if (listenForClients() == -1)
    {
        return -1;
    }

    return 0;
}

void TCPserver::serverClient(int client_fd, std::function<std::string(const std::string&)> requestHandler)
{
    std::string rawRequest(recvClient(client_fd));
    if (rawRequest.size()==0)
    {
        close(client_fd);return;
    }

    std::string rawRespons = requestHandler(rawRequest);

    ssize_t bytes_sent = send(client_fd, rawRespons.c_str(),rawRespons.size() , 0);
    if (bytes_sent == -1)
    {
        close(client_fd);
        return;
    }

    close(client_fd);
}


int TCPserver::createServerSocket()
{
    server_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        std::perror("socket failed");
        return -1;
    }

    return server_fd;
}

int TCPserver::bindServerSocket()
{
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int bind_result = ::bind(server_fd, (sockaddr*)&address, sizeof(address));
    if (bind_result == -1)
    {
        std::perror("bind failed");
        close(server_fd);
        server_fd = -1;
        return -1;
    }

    return 0;
}

int TCPserver::listenForClients()
{
    int listen_result = ::listen(server_fd, 5);

    if (listen_result == -1)
    {
        std::perror("listen failed");
        close(server_fd);
        server_fd = -1;
        return -1;
    }

    return 0;
}

int TCPserver::acceptClient()
{
    sockaddr_in client_address{};
    socklen_t client_address_size = sizeof(client_address);

    int client_fd = ::accept(
        server_fd,
        (sockaddr*)&client_address,
        &client_address_size
    );

    if (client_fd == -1)
    {
        std::perror("accept failed");
        return -1;
    }

    return client_fd;
}

std::string TCPserver::recvClient(int client_fd)
{
    char buffer[1024] = {0};

    ssize_t bytes_received = ::recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received == -1)
    {
        std::perror("recv failed");
        return "";
    }

    if (bytes_received == 0)
    {
        return "";
    }

    return std::string(buffer, bytes_received);
}