cmake_minimum_required(VERSION 3.16)

project(
    FinancialEye
    VERSION 0.0.1
    DESCRIPTION "Financial project. Personal expenses accounting system"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(FinancialEye
    src/main.cpp
    src/database.cpp
    src/userlogic.cpp
)

target_include_directories(FinancialEye PRIVATE include)

target_link_libraries(FinancialEye
    PRIVATE
        SQLite3::SQLite3
)

target_compile_options(FinancialEye PRIVATE -Wall -Wextra -Wpedantic)
