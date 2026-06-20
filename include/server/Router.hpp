#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include "server/HttpRequest.hpp"
#include "server/HttpResponse.hpp"
namespace server
{
    using RouteHandler = std::function<HttpResponse(const HttpRequest &)>;
    class Router
    {
    public:
        void get(const std::string &route, RouteHandler routerHandler);
        void post(const std::string &route, RouteHandler routeHandler);
        HttpResponse handle(const HttpRequest &request) const;

    private:
        std::string makeKey(
            const std::string &method,
            const std::string &path) const;

    private:
        std::unordered_map<std::string, RouteHandler> routesMap;
    };
}
