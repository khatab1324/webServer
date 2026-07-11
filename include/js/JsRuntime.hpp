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

        bool runCode(const std::string &code);
        bool runFile(const std::string &filePath);

    private:
        JSRuntime *runtime_ = nullptr;
        JSContext *context_ = nullptr;

        void registerConsole();
        void printException();
    };
}