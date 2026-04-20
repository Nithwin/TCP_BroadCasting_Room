cmake_minimum_required(VERSION 3.10)
project(TCP_BroadCasting_Room VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE Threads::Threads)

add_executable(client src/client.cpp)
target_link_libraries(client PRIVATE Threads::Threads)

