cmake_minimum_required(VERSION 3.28.1)

project(DATA_READER VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_PREFIX_PATH )

# find_package()

# include_directories()
# link_directories()

if (MSVC)
    add_compile_options(/wd4819)

endif ()

add_executable(dataReader
        test.cpp
        ../src/lexer.cpp
        ../src/parser.cpp
        ../src/utils.cpp
        ../src/ast.cpp
)

#option(USE_LEA "Use LEA implementation" OFF)
#option(USE_VSOP "Use VSOP implementation" ON)
#if (USE_LEA)
#    target_compile_definitions(dataReader PRIVATE LEA)
#elseif (USE_VSOP)
#    target_compile_definitions(dataReader PRIVATE VSOP)
#endif()

# target_link_libraries(MyTestExecutable PRIVATE ${Boost_LIBRARIES})
