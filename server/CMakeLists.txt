cmake_minimum_required(VERSION 3.12)
project(ChatSerer)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. SRCFILES)

add_executable(server ${SRCFILES})