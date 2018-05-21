cmake_minimum_required(VERSION 2.8)

project(tv_cast)

find_package(Boost 1.60 REQUIRED system thread)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${Boost_INCLUDE_DIR} .)

add_executable(${PROJECT_NAME}
    "main.cc"
    "com_ctrl.h"
    "com_ctrl.cc"
    "common/logger.h"
    "common/logger.cc"
    "server/server.h"
    "server/session.h"
    "server/packet_handler.h"
    "tcp/session.h"
    "tcp/session.cc"
    "tcp/server.h"
    "tcp/server.cc"
    "tcp/server_accessor.h"
    "http/packet.h"
    "http/parser.h"
    "http/parser.cc"
    "http/packet_handler.h"
    "http/packet_handler.cc"
    "logic/logic.h"
    "logic/logic.cc"
    "extra/http_parser.h"
    "extra/http_parser.c"
    )

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES} pthread)
