cmake_minimum_required(VERSION 3.12)
project(Chat)

option(DEBUG_mode "ON for debug or OFF for release" ON)
IF(DEBUG_mode)
    add_definitions(-DDEBUG)
ENDIF()

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# common
include_directories(extern/spdlog/include/)
ADD_SUBDIRECTORY(common)

# client and server
include_directories(common)
link_libraries(common)
link_libraries(pthread)
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)

