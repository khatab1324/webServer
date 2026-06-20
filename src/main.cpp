#include <iostream>
#include "server/TcpServer.hpp"
#include "server/HttpResponse.hpp"
int main(int argc, char *argv[])
{
    std::uint16_t port = 8080;
    server::TcpServer app(port);
    app.get("/about", [](const server::HttpRequest &request)
            { return server::HttpResponse::okText("This is the about page"); });
    app.post("/about", [](const server::HttpRequest &request)
             { return server::HttpResponse::okText("this post in the about page"); });
    app.start();
    return 0;
}