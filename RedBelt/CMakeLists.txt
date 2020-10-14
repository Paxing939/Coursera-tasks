cmake_minimum_required(VERSION 3.17)
project(RedBelt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")

add_executable(RedBelt "3 week/linked_list.cpp")