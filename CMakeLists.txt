project(server)


find_package(Threads)
add_executable(main main.cpp channel.h)
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
add_executable(socket socket.cpp)