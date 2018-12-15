cmake_minimum_required(VERSION 3.12)
project(ChatClient)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. SRCFILES)

add_executable(client ${SRCFILES})

