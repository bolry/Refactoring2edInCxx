cmake_minimum_required(VERSION 3.14)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    # Support Unix Makefiles and Ninja
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

project(Refactoring2edInCxx)

find_package(jsoncpp CONFIG REQUIRED)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
#set(THREADS_PREFER_PTHREAD_FLAG TRUE)
#find_package(Threads REQUIRED)

add_executable(the-main)
target_sources(the-main PRIVATE hmm.m.cpp)
target_link_libraries(the-main PRIVATE jsoncpp_lib_static)
