#include "js/JsRuntime.hpp"
#include <quickjs.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
// #include <server/Router.hpp>
#include <server/TcpServer.hpp>
#include <server/HttpRequest.hpp>
#include <server/HttpResponse.hpp>

namespace server
{
    static thread_local HttpResponse *currentResponse = nullptr;

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
    static JSValue sendResponse(
        JSContext *ctx,
        JSValueConst thisVal,
        int argc,
        JSValueConst *argv)
    {
        (void)thisVal;

        if (!currentResponse)
            return JS_ThrowInternalError(ctx, "there is no response have beed init");

        const char *body = JS_ToCString(ctx, argv[0]);
        if (!body)
            return JS_EXCEPTION;

        currentResponse->status(200)
            .setHeader("Content-Type", "text/plain")
            .send(body);

        JS_FreeCString(ctx, body);

        return JS_UNDEFINED;
    }

    static JSValue jsServerRouteHandlerGet(
        JSContext *ctx,
        JSValueConst thisVal,
        int argc,
        JSValueConst *argv)
    {
        (void)ctx;
        (void)thisVal;
        (void)argc;
        (void)argv;
        // my plan

        // argv[0] => router path "/hello"
        // argv[1] => function handler with (req,res)
        // req need to be js obj
        // res need to be js obj

        // you need to store the handler becuase with the request it come , c++ need to handle it from what
        //  it memo and you store in it

        TcpServer *server = static_cast<TcpServer *>(JS_GetContextOpaque(ctx));
        JSValue jsHandler = JS_DupValue(ctx, argv[1]);
        const char *route = JS_ToCString(ctx, argv[0]);
        server->get(route, [ctx, jsHandler](const HttpRequest &req) -> HttpResponse
                    {HttpResponse response;
                        currentResponse = &response;

                        JSValue reqObj = JS_NewObject(ctx);
                        JSValue resObj = JS_NewObject(ctx);
//here i deal with res
                        // JS_SetOpaque(resObj,&response);
                        JSValue args[] = { reqObj, resObj };
                        JS_SetPropertyStr(ctx, resObj, "send", JS_NewCFunction(ctx,sendResponse , "send", 1));
//here i deal with req
                        JS_SetPropertyStr(ctx,reqObj,"method",JS_NewString(ctx,req.method.c_str()));
                        JS_SetPropertyStr(ctx,reqObj,"body",JS_NewString(ctx,req.body.c_str()));
                        JS_SetPropertyStr(ctx,reqObj,"route",JS_NewString(ctx,req.route.c_str()));

                        JSValue result = JS_Call(ctx, jsHandler, JS_UNDEFINED, 2, args);//this will be like calling a function in js and if the handler return nothing it will be undifinde
    //like  server.get("/hello", function (req, res) {
    //   res.send("hello")
    // return 1; -> the result will be 1
    // })
                        JS_FreeValue(ctx, result);
                        JS_FreeValue(ctx, reqObj);
                        JS_FreeValue(ctx, resObj);
                        currentResponse = nullptr;

                        return response; });

        JS_FreeCString(ctx, route);

        return JS_UNDEFINED;
    }
    JsRuntime::JsRuntime(TcpServer &server) : server_(server)
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
        JS_SetContextOpaque(context_, &server_);
        registerConsole();
        registerServer();
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
    void JsRuntime::registerServer()
    {
        JSValue global = JS_GetGlobalObject(context_);
        JSValue server = JS_NewObject(context_);
        JS_SetPropertyStr(context_, server, "get", JS_NewCFunction(context_, jsServerRouteHandlerGet, "get", 1));
        JS_SetPropertyStr(context_, global, "server", server);
        JS_FreeValue(context_, global);
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
