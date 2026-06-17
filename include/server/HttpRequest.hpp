#pragma once

#include <string>

namespace server
{
    struct HttpRequest
    {
        std::string method;
        std::string path;
        std::string version;

        static HttpRequest parse(const std::string &rawRequest);
    };
}
