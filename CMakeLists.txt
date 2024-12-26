cmake_minimum_required(VERSION 3.16)
project(code-brok)

set(CMAKE_CXX_STANDARD 17) # Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Crow CONFIG REQUIRED)

add_executable(code-brok
    src/main.cpp
    src/routes.cpp
    src/utilities.cpp
    core/add.cpp
    core/db.cpp)
target_link_libraries(code-brok PRIVATE Crow::Crow)
target_include_directories(code-brok PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(code-brok PRIVATE ${CMAKE_SOURCE_DIR}/liblibsql.dylib)
