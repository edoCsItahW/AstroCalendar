cmake_minimum_required(VERSION 3.28.1)

project(ASTRO_CALENDER_TEST VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_PREFIX_PATH )

# find_package()

include_directories(../dataReader)

if (MSVC)
    add_compile_options(/wd4819)
endif ()

file(GLOB SOURCES "../src/*.cpp")
file(GLOB READER_SOURCES "../dataReader/src/*.cpp")

add_executable(astro_calender_test
        test.cpp
        ${SOURCES}
        ${READER_SOURCES}
)

# 链接动态库
#target_link_libraries(astro_calender_test PRIVATE ${CMAKE_SOURCE_DIR}/../dataReader/lib/data_reader_msvc.lib)
