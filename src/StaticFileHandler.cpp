#include "server/StaticFileHandler.hpp"

#include <fstream>
#include <sstream>

namespace server
{
    std::string StaticFileHandler::read(const std::string &requestPath)
    {
        std::string path = requestPath;

        if (path == "/")
        {
            path = "/index.html";
        }
        if(path=="/hello"){
            path="/hello.html";
        }
        std::ifstream file("public" + path, std::ios::binary);

        if (!file)
        {
            return "";
        }

        std::ostringstream content;
        content << file.rdbuf();

        return content.str();
    }
}
