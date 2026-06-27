#include <iostream>
#include "server/TcpServer.hpp"
#include "server/HttpResponse.hpp"
int main(int argc, char *argv[])
{
        std::uint16_t port = 8080;
        server::TcpServer app(port);
        // app.get("/about", [](const server::HttpRequest &request)
        //         { return server::HttpResponse::okText("This is the about page"); });
        // app.post("/about", [](const server::HttpRequest &request)
        //          { return server::HttpResponse::okText("this post in the about page"); });
        // app.get("/", [](const server::HttpRequest &request)
        //         { return server::HttpResponse::okText("wellcom in the webserver"); });
        app.get("/hello", [](const server::HttpRequest &request)
                { server::HttpResponse response;
                // response.status(200);
                // response.setHeader("Content-Type", "text/html");
                response.sendHtmlFile("/hello.html");
                return response; });
        app.start();
        return 0;
}