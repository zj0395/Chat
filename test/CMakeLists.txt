cmake_minimum_required(VERSION 3.12)
project(ChatTest)

set(CMAKE_CXX_STANDARD 11)

## extern library
link_directories($ENV{g_LIB_DIR})
link_directories($ENV{g_PROTOBUF_OUT}/lib)
link_directories($ENV{g_GTEST_OUT}/lib)
# spdlog
include_directories($ENV{g_CHAT_ROOT}/extern/spdlog/include)
# fmt
include_directories($ENV{g_CHAT_ROOT}/extern/fmt/include)
link_libraries(fmt)
# protobuf
include_directories($ENV{g_PROTOBUF_OUT}/include)
link_libraries(protobuf)
# protocol
include_directories($ENV{g_CHAT_ROOT}/src/common/protocol/protocol)
# gtest
include_directories($ENV{g_CHAT_ROOT}/extern/googletest/googletest/include)
link_libraries(gtest)
link_libraries(gtest_main)
# common
link_libraries(common)
link_libraries(pthread)


aux_source_directory(. SRCFILES)

add_executable(chat_test ${SRCFILES})