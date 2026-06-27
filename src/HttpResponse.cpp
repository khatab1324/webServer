#include "server/HttpResponse.hpp"
#include "server/StaticFileHandler.hpp"
#include <sstream>
#include <utility>
namespace server
{
    HttpResponse::HttpResponse()
        : statusCode_(200),
          statusText_("OK"),
          body_("")
    {
        }

    HttpResponse::HttpResponse(
        int statusCode,
        std::string statusText,
        std::string contentType,
        std::string body) : statusCode_(statusCode),
                            statusText_(std::move(statusText)),
                            body_(std::move(body))
    {
        headerType_["Content-Type"] = contentType;
    }
    HttpResponse HttpResponse::okText(const std::string &body)
    {
        return HttpResponse(200, "OK", "text/plain", body);
    }
    HttpResponse HttpResponse::okHtml(const std::string &filename)
    {
        std::string fileContent = StaticFileHandler::fileStreaming(filename);
        return HttpResponse(200, "OK", "text/html", fileContent);
    }
    HttpResponse HttpResponse::notFound()
    {
        std::string body = "404 Not Found";

        return HttpResponse(404, "Not Found", "text/plain", body);
    }
    HttpResponse HttpResponse::methodNotAllowed()
    {
        std::string body = "Method not allowed ^_^";
        return HttpResponse(405, "Not found", "text/plain", body);
    }
    std::string HttpResponse::toString() const
    {
        std::ostringstream response;

        response << "HTTP/1.1 " << statusCode_ << " " << statusText_ << "\r\n";
        // response << "Content-Type: " << "text/plain" << "\r\n";
        for (const auto &[name, value] : headerType_)
        {
            response << name << ": " << value << "\r\n";
        }
        response << "Content-Length: " << body_.size() << "\r\n";
        response << "Connection: close\r\n";
        response << "\r\n";
        response << body_;

        return response.str();
    }

    void HttpResponse::status(int statusCode)
    {
        // todo: add defualt status
        statusCode_ = statusCode;
    }
    void HttpResponse::status(int statusCode, const std::string &text)
    {
        statusText_ = text;
        statusCode_ = statusCode;
    }
    void HttpResponse::setHeader(const std::string &contantTypeName, const std::string &contantType)
    {
        headerType_[contantTypeName] = contantType;
    }
    void HttpResponse::send(const std::string &body)
    {
        body_ = body;
    }
    void HttpResponse::sendHtmlFile(const std::string &filename)
    {
        std::string fileContent = StaticFileHandler::fileStreaming(filename);
        if (!statusCode_)
        {
            statusCode_ = 200;
            statusText_ = "OK";
        }
        if (headerType_["Content-Type"].empty())
            setHeader("Content-Type", "text/html");

        body_ = fileContent;
    };
}
