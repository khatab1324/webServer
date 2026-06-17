#pragma once

#include <string>

namespace server
{
    class StaticFileHandler
    {
    public:
        static std::string read(const std::string &requestPath);
    };
}
