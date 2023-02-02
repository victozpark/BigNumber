cmake_minimum_required(VERSION 3.9)

project(BigNumber VERSION 0.9.0 DESCRIPTION "Big number")

set(CMAKE_CXX_STANDARD 11)

add_library(BigNumber SHARED ${CMAKE_SOURCE_DIR}/BigNumber.cpp)

set_target_properties(BigNumber PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(BigNumber PUBLIC ${CMAKE_SOURCE_DIR})

target_compile_options(BigNumber PRIVATE -Wall -Werror)

install(TARGETS BigNumber DESTINATION ${CMAKE_SOURCE_DIR}/release/lib)
install(FILES ${CMAKE_SOURCE_DIR}/BigNumber.hpp DESTINATION ${CMAKE_SOURCE_DIR}/release/include)
