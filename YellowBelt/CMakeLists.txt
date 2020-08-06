cmake_minimum_required(VERSION 3.16)
project(YellowBelt)

set(CMAKE_CXX_STANDARD 17)

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