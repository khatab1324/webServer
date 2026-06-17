#pragma once
#include <cstdint>

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

    private:
        std::uint16_t _port;
        int _serverSocket;

    private:
        void createSocket();
        void bindSocket();
        void listenForConnection();
        void acceptLoop();
    };
}
