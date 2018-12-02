cmake_minimum_required(VERSION 3.12)
project(Chat)

set(CMAKE_CXX_STANDARD 11)

include_directories(common)
ADD_SUBDIRECTORY(common)

link_directories(cmake-build-debug/common)
link_libraries(common)
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)