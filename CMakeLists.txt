cmake_minimum_required(VERSION 3.12)
project(Chat)

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(common)

link_libraries(common)
include_directories(common)
ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)

