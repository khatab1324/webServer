#pragma once

#include <string>

namespace server
{
    class HttpResponse
    {

    public:
        HttpResponse(int statusCode, std::string statusText, std::string contentType, std::string body);
        static HttpResponse okText(const std::string &body);
        static HttpResponse methodNotAllowed();
        static HttpResponse notFound();
        static HttpResponse okHtml(const std::string &filename);

        std::string toString() const;

    private:
        int statusCode_;
        std::string statusText_;
        std::string contentType_;
        std::string body_;
    };
};
