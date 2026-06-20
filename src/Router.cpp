#include "server/Router.hpp"

#include <utility>

namespace server
{
    void Router::get(const std::string &route, RouteHandler routerHandler)
    {
        routesMap[makeKey("GET", route)] = std::move(routerHandler);
    }
    void Router::post(const std::string &route, RouteHandler routerHandler)
    {
        routesMap[makeKey("POST", route)] = std::move(routerHandler);
    }
    HttpResponse Router::handle(const HttpRequest &request) const
    {
        const std::string key = makeKey(request.method, request.route);
        auto it = routesMap.find(key);
        if (it == routesMap.end())
        {
            return HttpResponse::notFound();
        }
        return it->second(request);
    };

    std::string Router::makeKey(const std::string &method,
                                const std::string &path) const
    {
        return method + " " + path;
    }

}