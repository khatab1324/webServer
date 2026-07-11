#include <iostream>
#include "server/TcpServer.hpp"
#include "server/HttpResponse.hpp"
#include "js/JsRuntime.hpp"

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
        try
        {
                server::JsRuntime js;
                js.runCode("console.print('hello','world','shheeee')");
                js.runCode("const x=3; x+3;4+4");
                std::cout
                    << "QuickJS runtime created successfully\n";
        }
        catch (const std::exception &error)
        {
                std::cerr << "Error: " << error.what() << '\n';
                return 1;
        }

        app.get("/hello", [](const server::HttpRequest &request)
                { server::HttpResponse response;
                // response.status(200);
                response.status(200).sendHtmlFile("/hello.html");
                return response; });
        app.post("/hello", [](const server::HttpRequest &request)
                 { server::HttpResponse response;
                        std::cout<<"body :"<<request.body<<std::endl;
                return response; });
        app.start();
        return 0;
}