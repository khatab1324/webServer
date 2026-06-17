#include "server/HttpResponse.hpp"

namespace server
{
    std::string HttpResponse::okHtml(const std::string &body)
    {
        return "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: " +
               std::to_string(body.size()) + "\r\n"
                                             "\r\n" +
               body;
    }

    std::string HttpResponse::notFound()
    {
        std::string body = "404 Not Found";

        return "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/plain\r\n"
               "Content-Length: " +
               std::to_string(body.size()) + "\r\n"
                                             "\r\n" +
               body;
    }
    std::string HttpResponse::methodNotAllowed()
    {
        std::string body = "Method not allowed ^_^";

        return "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/plain\r\n"
               "Content-Length: " +
               std::to_string(body.size()) + "\r\n"
                                             "\r\n" +
               body;
    }
}
