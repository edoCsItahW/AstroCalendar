cmake_minimum_required(VERSION 3.28.1)

project(ASTRO_CALENDER VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_PREFIX_PATH )

# find_package()

# include_directories()
# link_directories()

add_executable(astroCalender
        main.cpp
        src/constant.cpp
)

# target_link_libraries(MyTestExecutable PRIVATE ${Boost_LIBRARIES})
