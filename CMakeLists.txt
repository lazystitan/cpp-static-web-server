cmake_minimum_required(VERSION 3.10)
project(server)


find_package(Threads)
add_executable(main main.cpp channel.h)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
add_executable(socket_server socket_server.cpp)
add_executable(socket_client socket_client.cpp)