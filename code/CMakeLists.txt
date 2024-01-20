cmake_minimum_required (VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)

project ("robot")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(robot
    robot.cpp
    robot.h
)
target_link_libraries(robot PUBLIC SDL2::SDL2 SDL2_image::SDL2_image)

add_executable(create_fields create_fields.cpp)
target_link_libraries(create_fields robot)

add_executable(task1 task1_series_of_commands.cpp)
target_link_libraries(task1 robot)

add_executable(task2 task2_make_your_own_commands.cpp)
target_link_libraries(task2 robot)

add_executable(task3 task3_change_the_order_of_commands.cpp)
target_link_libraries(task3 robot)
