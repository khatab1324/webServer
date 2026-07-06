#pragma once
#include <string>

struct JSRuntime;
struct JSContext;
namespace server
{
    class JsRuntime
    {
    public:
        JsRuntime();
        ~JsRuntime();

        JsRuntime(const JsRuntime &) = delete;
        JsRuntime &operator=(const JsRuntime &) = delete;

        bool runFile(const std::string &filePath);

    private:
        JSRuntime *runtime_;
        JSContext *context_;

        void registerConsole();
        void printException();
    };
}