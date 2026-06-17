#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include "server/HttpRequest.hpp"

namespace server
{
    using RouteHandler = std::function<std::string(const HttpRequest &)>;
    class Router
    {
    public:
        void get(std::string &route, RouteHandler routerHandler);
        std::optional<std::string> handle(const HttpRequest &request) const;

    private:
        std::string makeKey(
            const std::string &method,
            const std::string &path) const;

    private:
        std::unordered_map<std::string, RouteHandler> routesMap;
    };
}
