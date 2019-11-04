cmake_minimum_required(VERSION 3.1)
project(3d_world)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_GXX_FLAGS "-Wall -Wextra -Wconversion  -pedantic -std=gnu11")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(pencil src/pencil_draw.cpp)
target_link_libraries(pencil ${OpenCV_LIBS})

add_executable(puzzle src/puzzle_effect.cpp)
target_link_libraries(puzzle ${OpenCV_LIBS})

add_executable(cubes src/cubism.cpp)
target_link_libraries(cubes ${OpenCV_LIBS})

add_executable(oil src/oil.cpp)
target_link_libraries(oil ${OpenCV_LIBS})
