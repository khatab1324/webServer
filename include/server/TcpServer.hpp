#pragma once
#include <cstdint>
#include "server/Router.hpp"

namespace server
{
    class TcpServer
    {
    public:
        explicit TcpServer(std::uint16_t port);
        ~TcpServer();

        TcpServer(const TcpServer &) = delete;
        TcpServer &operator=(const TcpServer &) = delete;

        void start();
        void get(const std::string &route, RouteHandler handler);
        void post(const std::string &route, RouteHandler handler);

    public:
        std::uint16_t _port;
        int _serverSocket;
        Router _router;

    private:
        void createSocket();
        void bindSocket();
        void listenForConnection();
        void acceptLoop();
        void handleClient(int clientSocket);
    };
}
