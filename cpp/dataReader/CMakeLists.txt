cmake_minimum_required(VERSION 3.28.1)

# 项目名
project(DATA_READER_LIB VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_GENERATOR_PLATFORM "x64")

# 生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# 库名
if (MSVC)
    set(LIB_NAME "data_reader_msvc")
else ()
    set(LIB_NAME "data_reader_gcc")
endif ()

if (MSVC)
    add_compile_options(/wd4819)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

file(GLOB_RECURSE SOURCE_FILES "./src/*.cpp")

add_library(${LIB_NAME} SHARED
        ${SOURCE_FILES}
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# 如有链接动态库(dll)需补全(如: ws2_32.dll -> ws2_32)
#target_link_libraries(${LIB_NAME} PRIVATE )

# 导出宏
target_compile_definitions(${LIB_NAME} PRIVATE ALLOW_EXPORT)

function(apply_max_optimization TARGET)
    if (MSVC)
        add_compile_options(/wd4819)

        target_compile_options(${TARGET} PRIVATE
                "$<$<CONFIG:Release>:/Ox;/fp:fast;/GL>"
                "$<$<CONFIG:RelWithDebInfo>:/Ox;/fp:fast;/GL>"
        )
        set_target_properties(${TARGET} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
                INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${TARGET} PRIVATE
                "$<$<CONFIG:Release>:-Ofast;-march=native;-flto=auto;-ffast-math>"
                "$<$<CONFIG:RelWithDebInfo>:-Ofast;-march=native;-flto=auto;-ffast-math;-g>"
        )
        set_target_properties(${TARGET} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
                INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )
    endif ()
endfunction()

set(CMAKE_MAKE_PROGRAM "D:/Program Files/Jetbrains/Clion/CLion250101/bin/mingw/bin/mingw32-make.exe")

apply_max_optimization(${LIB_NAME})

# MSCV: 1. cmake -B build -S . 2. cmake --build build --config Release
# MinGW: 1. mkdir build 2. cd build 3. cmake .. -G "MinGW makefiles" 4. mingw32-make