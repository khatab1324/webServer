#pragma once

#include <string>

namespace server
{
    class HttpResponse
    {
    public:
        static std::string okHtml(const std::string &body);
        static std::string methodNotAllowed();
        static std::string notFound();
    };
}
