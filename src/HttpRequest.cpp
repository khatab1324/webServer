#include "server/HttpRequest.hpp"

#include <sstream>

namespace server
{
    HttpRequest HttpRequest::parse(const std::string &rawRequest)
    {
        HttpRequest request{};
        std::size_t headerEnd = rawRequest.find("\r\n\r\n");

        std::istringstream stream(rawRequest);
        stream >> request.method >> request.route >> request.version;
        if (headerEnd != std::string::npos)
        {
            request.body = rawRequest.substr(headerEnd + 4);
        }

        return request;
    }
}
