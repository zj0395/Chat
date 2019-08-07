cmake_minimum_required(VERSION 3.12)
project(ChatTest)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CHAT_ROOT}/extern/googletest/googletest/include)
link_directories(${GTEST_DIR}/lib)
link_libraries(gtest)
link_libraries(gtest_main)

aux_source_directory(. SRCFILES)

add_executable(chat_test ${SRCFILES})