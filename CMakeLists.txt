cmake_minimum_required(VERSION 3.20)

project(webServer VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(webServer
    src/main.cpp
    src/TcpServer.cpp
    src/HttpRequest.cpp
    src/HttpResponse.cpp
    src/StaticFileHandler.cpp
)

target_include_directories(webServer
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(webServer
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
