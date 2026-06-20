#include "server/HttpResponse.hpp"
#include <sstream>
#include <utility>
namespace server
{
    HttpResponse::HttpResponse(
        int statusCode,
        std::string statusText,
        std::string contentType,
        std::string body) : statusCode_(statusCode),
                            statusText_(std::move(statusText)),
                            contentType_(std::move(contentType)),
                            body_(std::move(body)) {}
    HttpResponse HttpResponse::okHtml(const std::string &body)
    {
        return HttpResponse(200, "ok", "plain/text", body);
    }

    HttpResponse HttpResponse::notFound()
    {
        std::string body = "404 Not Found";

        return HttpResponse(200, "OK", "text/plain", body);
    }
    HttpResponse HttpResponse::methodNotAllowed()
    {
        std::string body = "Method not allowed ^_^";
        return HttpResponse(404, "Not found", "text/plain", body);
    }
    std::string HttpResponse::toString() const
    {
        std::ostringstream response;

        response << "HTTP/1.1 " << statusCode_ << " " << statusText_ << "\r\n";
        response << "Content-Type: " << contentType_ << "\r\n";
        response << "Content-Length: " << body_.size() << "\r\n";
        response << "Connection: close\r\n";
        response << "\r\n";
        response << body_;

        return response.str();
    }
}
