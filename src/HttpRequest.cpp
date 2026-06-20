#include "server/HttpRequest.hpp"

#include <sstream>

namespace server
{
    HttpRequest HttpRequest::parse(const std::string &rawRequest)
    {
        HttpRequest request{};

        std::istringstream stream(rawRequest);
        stream >> request.method >> request.route >> request.version;

        return request;
    }
}
