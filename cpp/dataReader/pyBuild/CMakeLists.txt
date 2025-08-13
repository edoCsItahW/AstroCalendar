cmake_policy(SET CMP0057 NEW)
# 3.29是你的cmake版本
cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(DATA_READER CXX)  # 大小项目名

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Python_ROOT_DIR "E:/Anaconda")  # 显式设置Python根目录
set(Python_EXECUTABLE "E:/Anaconda/python.exe")  # 显式指定解释器路径

function(apply_max_optimization TARGET)
    if (MSVC)
        add_compile_options(/wd4819)

        target_compile_options(${TARGET} PRIVATE
                "$<$<CONFIG:Release>:/O2;/fp:fast;/GL>"
                "$<$<CONFIG:RelWithDebInfo>:/O2;/fp:fast;/GL>"
        )
        set_target_properties(${TARGET} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
                INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${TARGET} PRIVATE
                "$<$<CONFIG:Release>:-O3;-march=native;-flto=auto;-ffast-math>"
                "$<$<CONFIG:RelWithDebInfo>:-O3;-march=native;-flto=auto;-ffast-math;-g>"
        )
        set_target_properties(${TARGET} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
                INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )
    endif ()
endfunction()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS_RELEASE_INIT "/O2 /Ob2 /DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT "/O2 /Ob2 /DNDEBUG")


set(pybind11_DIR "E:/Anaconda/Lib/site-packages/pybind11/share/cmake/pybind11")

find_package(Python 3.12 COMPONENTS Interpreter Development REQUIRED)

execute_process(
        COMMAND "${Python_EXECUTABLE}" "-c" "import platform; print(platform.architecture()[0])"
        OUTPUT_VARIABLE PYTHON_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT PYTHON_ARCH STREQUAL "64bit")
    message(FATAL_ERROR "Required 64-bit Python but found ${PYTHON_ARCH} version")
endif ()

find_package(pybind11 CONFIG REQUIRED)

# 注意：由于这是add_module，所以不会生成可执行文件和cmake目标，如需要调试请使用add_executable
pybind11_add_module(dataReader
        ../src/lexer.cpp
        ../src/ast.cpp
        ../src/parser.cpp
        ../src/utils.cpp
        build.cpp
)  # 两个参数,小写的项目名和主cpp文件名

apply_max_optimization(dataReader)  # 优化选项
