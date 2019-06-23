cmake_minimum_required(VERSION 3.12)
project(common)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. SRCFILES)
aux_source_directory(${CHAT_ROOT}/src/common/protocol/protocol SRCFILES)

add_library(common STATIC ${SRCFILES})

