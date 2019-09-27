cmake_minimum_required(VERSION 3.10)
project(server)


find_package(Threads)
add_executable(main main.cpp channel.h)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
add_executable(socket_server socket_server.cpp)
add_executable(socket_client socket_client.cpp)
add_executable(server_test server_test.cpp)
add_executable(wrapper_client_test wrapper_client_test.cpp client.h)
add_executable(wrapper_server_test wrapper_server_test.cpp server.h request.h)