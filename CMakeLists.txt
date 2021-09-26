cmake_minimum_required(VERSION 2.0)
project(primo)

#启用 Makefile 构建的冗长输出
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(
    LIB_SRC
    src/log/Logger.cpp
)

add_library(primo SHARED ${LIB_SRC})

add_executable(test tests/testlog.cpp)
add_dependencies(test primo)
target_link_libraries(test primo)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)