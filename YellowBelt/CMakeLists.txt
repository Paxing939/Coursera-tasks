cmake_minimum_required(VERSION 3.16)
project(YellowBelt)

set(CMAKE_CXX_STANDARD 17)

add_executable(YellowBelt
        "6 week/Date.cpp"
        "6 week/main.cpp"
        "6 week/Database.cpp"
        "6 week/ConditionParser.cpp"
        "6 week/Node.cpp"
        "6 week/DateParser.cpp"
        "6 week/Token.cpp"
        "6 week/ConditionParserTest.cpp"
        )