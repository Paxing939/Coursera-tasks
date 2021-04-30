cmake_minimum_required(VERSION 3.17)
project(RedBelt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")

add_executable(RedBelt "6 week/first_part/main.cpp" "6 week/first_part/parse.cpp" "6 week/first_part/search_server.cpp"
        "6 week/first_part/RequestTimeEstimator.cpp" "6 week/first_part/RequestTimeEstimator.h")
