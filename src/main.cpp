#include "server/TcpServer.hpp"
#include <iostream>
int main(int argc, char *argv[])
{
    std::uint16_t port = 8080;
    server::TcpServer app(port);
    app.start();
}