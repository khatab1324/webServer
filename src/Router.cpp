#include "server/Router.hpp"

#include <utility>

namespace server
{
    void Router::get(std::string &route, RouteHandler routerHandler)
    {
        routesMap[makeKey("GET", route)] = std::move(routerHandler);
    }
    std::string makeKey(const std::string &method,
                        const std::string &path)
    {
        return method + " " + path;
    }
}