#include "server/Router.hpp"

#include <utility>

namespace server
{
    void Router::get(std::string &route, RouteHandler routerHandler)
    {
        routesMap[makeKey("GET", route)] = std::move(routerHandler);
    }
    void Router::post(std::string &route, RouteHandler routerHandler)
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

    std::string makeKey(const std::string &method,
                        const std::string &path)
    {
        return method + " " + path;
    }

}