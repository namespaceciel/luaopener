cmake_minimum_required(VERSION 3.27)
project(luaopener)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(third_party/lua)

add_executable(luaopener main.cpp)

target_include_directories(luaopener PUBLIC include)

target_link_libraries(luaopener PUBLIC lua_static)