#pragma once

#include <string>
#include <unordered_map>

namespace server
{
    class HttpResponse
    {
    public:
        HttpResponse();
        HttpResponse(int statusCode, std::string statusText, std::string contentType, std::string body);

    public:
        HttpResponse &status(int statusCode);
        HttpResponse &status(int statusCode, const std::string &text);
        HttpResponse &setHeader(const std::string &contantTypeName, const std::string &contantType);
        HttpResponse &send(const std::string &body);
        HttpResponse &sendHtmlFile(const std::string &filename);

    public:
        static HttpResponse okText(const std::string &body);
        static HttpResponse methodNotAllowed();
        static HttpResponse notFound();
        static HttpResponse okHtml(const std::string &filename);

        std::string toString() const;

    public:
        int statusCode_;
        std::string statusText_;
        std::unordered_map<std::string, std::string> headerType_;
        std::string body_;
    };
};
