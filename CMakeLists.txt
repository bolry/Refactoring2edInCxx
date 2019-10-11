cmake_minimum_required(VERSION 3.12)
project(Refactoring2edInCxx_git)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 98)

add_executable(Refactoring2edInCxx_git main.cpp statement.cpp statement.h invoice.cpp invoice.h)
