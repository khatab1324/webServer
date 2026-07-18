#pragma once
#include <string>

struct JSRuntime;
struct JSContext;
namespace server
{
    class TcpServer;
    class JsRuntime
    {
    public:
        explicit JsRuntime(TcpServer &server_);
        ~JsRuntime();

        JsRuntime(const JsRuntime &) = delete;
        JsRuntime &operator=(const JsRuntime &) = delete;

        bool runCode(const std::string &code);
        bool runFile(const std::string &filePath);

    private:
        JSRuntime *runtime_ = nullptr;
        JSContext *context_ = nullptr;
        TcpServer &server_;

        void registerConsole();
        void registerServer();

        void printException();
    };
}