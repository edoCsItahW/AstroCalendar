cmake_policy(SET CMP0057 NEW)
# 3.29是你的cmake版本
cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(DATAREADER)  # 大小项目名

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/wd4819)
endif()

set(pybind11_DIR "E:/Anaconda/Lib/site-packages/pybind11/share/cmake/pybind11")

# 3.11是你的python解释器版本
find_package(Python 3.12 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(
    dataReader
        build.cpp
        ../src/lexer.cpp
        ../src/parser.cpp
        ../src/utils.cpp
        ../src/ast.cpp
)  # 两个参数,小写的项目名和主cpp文件名

