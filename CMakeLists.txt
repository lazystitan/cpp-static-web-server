cmake_minimum_required(VERSION 3.10)
project(server)

find_package (Threads)
add_executable(mult_thread_server mult_thread_server.cpp)
target_link_libraries (mult_thread_server ${CMAKE_THREAD_LIBS_INIT})

