cmake_minimum_required(VERSION 2.0)
project(primo)

#include (cmake/utils.cmake)

#启用 Makefile 构建的冗长输出
set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_CXX_FLAGS "$ENV{CXX_FLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated 
    #-Werror -Wno-unused-function -Wno-builtin-macro-redefined")

link_directories(/usr/lib64)
find_library(YAMLCPP libyaml-cpp)


set(
    LIB_SRC
    primo/log/Logger.cpp
    primo/util.cpp
    primo/config.cpp
    primo/thread.cpp
)

add_library(primo SHARED ${LIB_SRC})

set (LIB_LIB
    primo
    pthread
    ${YAMLCPP}
)

add_executable(test_log tests/testlog.cpp)
add_dependencies(test_log primo)
target_link_libraries(test_log ${LIB_LIB})
#force_redefine_file_macro_for_sources(primo)

add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config primo)
target_link_libraries(test_config ${LIB_LIB})
#force_redefine_file_macro_for_sources(test_config)

add_executable(test_thread tests/test_thread.cpp)
add_dependencies(test_thread primo)
target_link_libraries(test_thread ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)