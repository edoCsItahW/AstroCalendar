cmake_minimum_required(VERSION 3.28.1)

project(DATA_READER VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_PREFIX_PATH )

# find_package()

# include_directories()
# link_directories()

add_executable(dataReader
        test.cpp
        ../src/lexer.cpp
        ../src/parser.cpp
        ../src/utils.cpp
        ../src/ast.cpp
)

# target_link_libraries(MyTestExecutable PRIVATE ${Boost_LIBRARIES})
