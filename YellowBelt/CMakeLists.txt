cmake_minimum_required(VERSION 3.16)
project(YellowBelt)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")

add_executable(YellowBelt
        "6 week/date.cpp"
        "6 week/main.cpp"
        "6 week/database.cpp"
        "6 week/condition_parser.cpp"
        "6 week/node.cpp"
        "6 week/date_parser.cpp"
        "6 week/token.cpp"
        "6 week/condition_parser_test.cpp"
        )