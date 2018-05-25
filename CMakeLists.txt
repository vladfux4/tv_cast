cmake_minimum_required(VERSION 2.8)
project(tv_cast)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_LIST_DIR}/cmake/module"
    CACHE STRING "Modules for CMake" FORCE)
find_package(Boost 1.60 REQUIRED system thread)
find_package(glog 0.3.5 REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${Boost_INCLUDE_DIR} .)

add_executable(${PROJECT_NAME}
    "main.cc"
    "com_ctrl.h"
    "com_ctrl.cc"
    "common/logger.h"
    "common/logger.cc"
    "net/session_dispatcher.h"
    "net/session_dispatcher.cc"
    "net/session.h"
    "net/session_handler.h"
    "net/session_accessor.h"
    "tcp/session.h"
    "tcp/session.cc"
    "tcp/server.h"
    "tcp/server.cc"
    "tcp/client.h"
    "tcp/client.cc"
    "http/packet.h"
    "http/packet.cc"
    "http/parser.h"
    "http/parser.cc"
    "http/session_handler.h"
    "http/session_handler.cc"
    "logic/server_logic.h"
    "logic/server_logic.cc"
    "logic/client_logic.h"
    "logic/client_logic.cc"
    "extra/http_parser.h"
    "extra/http_parser.c"
    )

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC ${Boost_LIBRARIES} glog pthread)
