#include "js/JsRuntime.hpp"
#include <quickjs.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
namespace server
{
    static JSValue jsConsoleLog(
        JSContext *ctx,
        JSValueConst thisVal,
        int argc,
        JSValueConst *argv)
    {
        (void)thisVal;

        for (int i = 0; i < argc; ++i)
        {
            const char *text = JS_ToCString(ctx, argv[i]);

            if (text)
            {
                std::cout << text;
                JS_FreeCString(ctx, text);
            }

            if (i + 1 < argc)
            {
                std::cout << " ";
            }
        }

        std::cout << '\n';

        return JS_UNDEFINED;
    }
    JsRuntime::JsRuntime()
    {
        runtime_ = JS_NewRuntime();
        if (!runtime_)
            throw std::runtime_error("Failed to create runtime for QuickJS");

        context_ = JS_NewContext(runtime_);

        if (!context_)
        {
            JS_FreeRuntime(runtime_);
            throw std::runtime_error("Failed to create context for QuickJS");
        }
        registerConsole();
    }
    JsRuntime::~JsRuntime()
    {
        if (context_)
        {
            JS_FreeContext(context_);
        }

        if (runtime_)
        {
            JS_FreeRuntime(runtime_);
        }
    }
    bool JsRuntime::runCode(const std::string &code)
    {
        JSValue result = JS_Eval(context_, code.c_str(), code.size(), "", JS_EVAL_TYPE_GLOBAL);
        if (JS_IsException(result))
        {
            printException();
            JS_FreeValue(context_, result);
            return false;
        }
        // TODO:here just for print i will remove it
        const char *resultString = JS_ToCString(context_, result);
        if (resultString)
        {
            std::cout << "js result: " << resultString << std::endl;
            JS_FreeCString(context_, resultString);
        }
        JS_FreeValue(context_, result);
        return true;
    }
    bool JsRuntime::runFile(const std::string &filename)
    {
        std::string mainPath = "scripts/";
        std::ifstream file(mainPath + filename);
        if (!file.is_open())
        {
            std::cerr << "can't open js file" << filename << std::endl;
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string code = buffer.str();

        return runCode(code);
    }
    void JsRuntime::printException()
    {
        JSValue exception = JS_GetException(context_);

        const char *message = JS_ToCString(context_, exception);

        if (message)
        {
            std::cerr << "error: " << message << '\n';
            JS_FreeCString(context_, message);
        }

        JS_FreeValue(context_, exception);
    }
    void JsRuntime::registerConsole()
    {
        JSValue global = JS_GetGlobalObject(context_);
        JSValue console = JS_NewObject(context_);
        JS_SetPropertyStr(
            context_,
            console,
            "print",
            JS_NewCFunction(context_, jsConsoleLog, "print", 1));
        JS_SetPropertyStr(context_, global, "console", console);
        JS_FreeValue(context_, global);
    }

}