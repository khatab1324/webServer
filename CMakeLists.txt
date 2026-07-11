cmake_minimum_required(VERSION 3.20)

project(webServer VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(QUICKJS_DIR ${CMAKE_SOURCE_DIR}/engin/quickjs)

set(QUICKJS_SOURCES
    ${QUICKJS_DIR}/quickjs.c
    ${QUICKJS_DIR}/libregexp.c
    ${QUICKJS_DIR}/libunicode.c
)
if(EXISTS ${QUICKJS_DIR}/dtoa.c)
    list(APPEND QUICKJS_SOURCES ${QUICKJS_DIR}/dtoa.c)
endif()

if(EXISTS ${QUICKJS_DIR}/libbf.c)
    list(APPEND QUICKJS_SOURCES ${QUICKJS_DIR}/libbf.c)
endif()

add_library(quickjs STATIC
    ${QUICKJS_SOURCES}
)

target_include_directories(quickjs
    PUBLIC
        ${QUICKJS_DIR}
)

target_compile_definitions(quickjs
    PRIVATE
        CONFIG_VERSION=\"quickjs-ng\"
)

target_link_libraries(quickjs
    PUBLIC
        m
        pthread
        ${CMAKE_DL_LIBS}
)

add_executable(webServer
    src/main.cpp
    src/TcpServer.cpp
    src/HttpRequest.cpp
    src/HttpResponse.cpp
    src/StaticFileHandler.cpp
    src/Router.cpp
    src/js/JsRuntime.cpp

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

target_link_libraries(webServer
    PRIVATE
        quickjs
)