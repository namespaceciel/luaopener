cmake_minimum_required(VERSION 3.27)
project(luaopener)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(third_party/lua)

add_library(luaopener STATIC
        src/luaopener.cpp
        src/luavalue.cpp)

target_include_directories(luaopener PUBLIC include)

target_link_libraries(luaopener PUBLIC lua_static)

string(COMPARE EQUAL ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR} PROJECT_IS_TOP_LEVEL)

option(LUAOPENER_BUILD_TESTS "Build unit tests" ${PROJECT_IS_TOP_LEVEL})

if (LUAOPENER_BUILD_TESTS)
    add_executable(luaopener_test
            test/all.cpp
    )

    target_link_libraries(luaopener_test luaopener)
endif ()