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
    "anet/net/session_handler.h"
    "anet/net/session_handler.cc"
    "anet/net/session.h"
    "anet/net/session_observer.h"
    "anet/net/session_accessor.h"
    "anet/net/packet_observer.h"

    "anet/tcp/session.h"
    "anet/tcp/session.cc"
    "anet/tcp/server.h"
    "anet/tcp/server.cc"
    "anet/tcp/client.h"
    "anet/tcp/client.cc"

    "anet/udp/server.h"
    "anet/udp/server.cc"

    "anet/http/packet.h"
    "anet/http/packet.cc"
    "anet/http/parser.h"
    "anet/http/parser.cc"
    "anet/http/session_observer.h"
    "anet/http/session_observer.cc"
    "anet/extra/http_parser.h"
    "anet/extra/http_parser.c"

    "logic/server_logic.h"
    "logic/server_logic.cc"
    "logic/client_logic.h"
    "logic/client_logic.cc"

    "common/logger.h"
    "common/logger.cc"
    "com_ctrl.h"
    "com_ctrl.cc"
    "main.cc"
    )

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC ${Boost_LIBRARIES} glog pthread)
