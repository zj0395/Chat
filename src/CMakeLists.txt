cmake_minimum_required(VERSION 3.12)
project(Chat)

option(DEBUG_mode "ON for debug or OFF for release" ON)
IF(DEBUG_mode)
    add_definitions(-DDEBUG)
ENDIF()

set(CMAKE_CXX_STANDARD 11)
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# extern library
include_directories($ENV{g_CHAT_ROOT}/extern/spdlog/include)

include_directories($ENV{g_CHAT_ROOT}/extern/fmt/include)
link_directories(${LIBRARY_OUTPUT_PATH})
link_libraries(fmt)

include_directories(${PROTOBUF_PATH}/include)
link_directories(${PROTOBUF_PATH}/lib)
link_libraries(protobuf)

# protocol
include_directories(${ROOT}/common/protocol/protocol)

# common
include_directories(common/protocol)
ADD_SUBDIRECTORY(common)

# client and server

# include
include_directories(common)
# library
link_libraries(common)
link_libraries(pthread)
# src
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)

