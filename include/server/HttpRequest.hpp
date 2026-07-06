#pragma once
#include <unordered_map>

#include <string>

namespace server
{
    struct HttpRequest
    {
        std::string method;
        std::string route;
        std::string version;
        std::string body;
        std::unordered_map<std::string, std::string> headers;
        std::unordered_map<std::string, std::string> queryParams;

        static HttpRequest parse(const std::string &rawRequest);
    };
}
