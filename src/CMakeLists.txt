cmake_minimum_required(VERSION 3.12)
project(Chat)

option(DEBUG_mode "ON for debug or OFF for release" ON)
IF(DEBUG_mode)
    add_definitions(-DDEBUG)
ENDIF()

set(CMAKE_CXX_STANDARD 11)


## extern library
link_directories(${LIBRARY_OUTPUT_PATH})
# spdlog
include_directories(${CHAT_ROOT}/extern/spdlog/include)
# fmt
include_directories(${CHAT_ROOT}/extern/fmt/include)
link_libraries(fmt)
# protobuf
include_directories(${PROTOBUF_PATH}/include)
link_directories(${PROTOBUF_PATH}/lib)
link_libraries(protobuf)
# protocol
include_directories(${CHAT_ROOT}/src/common/protocol/protocol)
# threadpool
include_directories(${CHAT_ROOT}/extern/)


## common
include_directories(common)
include_directories(common/connector)
include_directories(common/protocol)
include_directories(common/message)
ADD_SUBDIRECTORY(common)


## client and server
# library
link_libraries(common)
link_libraries(pthread)
# src
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)
