cmake_minimum_required(VERSION 3.12)
project(Chat)

option(DEBUG_mode "ON for debug or OFF for release" ON)
IF(DEBUG_mode)
    add_definitions(-DDEBUG)
ENDIF()

set(CMAKE_CXX_STANDARD 11)

# extern
include_directories(extern/spdlog/include)
include_directories(extern/fmt/include)
link_directories(${LIBRARY_OUTPUT_PATH})
link_libraries(fmt)

# common
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

