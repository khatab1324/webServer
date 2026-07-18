#include "server/TcpServer.hpp"

#include "server/HttpRequest.hpp"
#include "server/HttpResponse.hpp"
#include "server/StaticFileHandler.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

namespace server
{
    TcpServer::TcpServer(std::uint16_t port)
    {
        _port = port;
        _serverSocket = -1; // bro this for safe destructor
    }
    TcpServer::~TcpServer()
    {
        if (_serverSocket != -1)
        {
            close(_serverSocket);
        }
    }
    void TcpServer::start()
    {
        createSocket();
        bindSocket();
        listenForConnection();
        acceptLoop();
    }
    void TcpServer::createSocket()
    {
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        // std::cout << _serverSocket << AF_INET6 << std::endl;
        if (_serverSocket == -1)
        {
            throw std::runtime_error("Faild to create socket");
        }
        int reuse = 1;
        if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
        {
            throw std::runtime_error("Failed to reuse socket option");
        }
    }
    void TcpServer::bindSocket()
    {
        sockaddr_in serverAddress{};
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(_port);

        int result = bind(
            _serverSocket,
            reinterpret_cast<sockaddr *>(&serverAddress),
            sizeof(serverAddress));

        // std::cout << result << std::endl;
        if (result == -1)
        {
            throw std::runtime_error("Failed to bind socket");
        }
    }
    void TcpServer::listenForConnection()
    {
        int result = listen(_serverSocket, SOMAXCONN);

        if (result == -1)
        {
            throw std::runtime_error("Failed to listen on socket");
        }

        std::cout << "Server listening on port " << _port << '\n';
    }
    void TcpServer::acceptLoop()
    {
        while (true)
        {
            sockaddr_in clientAddress{};
            socklen_t clientAddressSize = sizeof(clientAddress);

            int clientSocket = accept(
                _serverSocket,
                reinterpret_cast<sockaddr *>(&clientAddress),
                &clientAddressSize);

            if (clientSocket == -1)
            {
                std::cerr << "Failed to accept client connection\n";
                continue;
            }
            // char clientIp[INET_ADDRSTRLEN];
            // inet_ntop(
            //     AF_INET,
            //     &clientAddress.sin_addr,
            //     clientIp,
            //     sizeof(clientIp));

            // std::cout << "Client connected: " << clientIp << '\n';

            // todo : for some resone quick js can't catch the clientThread , fix this by making quick js running on worker thread not main thread
            //  this need to be in the client handle
            //  std::thread clientThread(
            //      &TcpServer::handleClient, this, clientSocket);
            //  clientThread.detach();
            handleClient(clientSocket);
        }
    }
    void TcpServer::handleClient(int clientSocket)
    {

        char buffer[4096]{};

        ssize_t bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytesReceived > 0)
        {
            // buffer[bytesReceived] = '\0';
            std::string rawRequest(buffer);
            HttpRequest req = HttpRequest::parse(rawRequest);
            std::cout << "Method: " << req.method << '\n';
            std::cout << "Path: " << req.route << '\n';

            // std::string fileContent = StaticFileHandler::read(req.route);
            std::cout << "Received request:\n";
            std::cout << buffer << '\n';

            HttpResponse response = _router.handle(req);
            if (req.method != "GET" && req.method != "POST")
                response = HttpResponse::methodNotAllowed();
            else
            {
                // if (fileContent.empty())
                // {
                //     response = HttpResponse::notFound();
                // }
                // else
                // {
                // response = HttpResponse::okText(fileContent);
                // }
            }
            std::string rawRes = response.toString();
            send(clientSocket, rawRes.c_str(), rawRes.size(), 0);
        }

        close(clientSocket);
    }
    void TcpServer::get(const std::string &route, RouteHandler handler)
    {
        _router.get(route, std::move(handler));
    }

    void TcpServer::post(const std::string &route, RouteHandler handler)
    {
        _router.post(route, std::move(handler));
    }
}
