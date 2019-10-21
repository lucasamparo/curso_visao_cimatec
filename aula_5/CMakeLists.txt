cmake_minimum_required(VERSION 3.1)
project(3d_world)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_GXX_FLAGS "-Wall -Wextra -Wconversion  -pedantic -std=gnu11")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(stereo src/stereo_vision.cpp)
target_link_libraries(stereo ${OpenCV_LIBS})

add_executable(epipolar src/epipolar.cpp)
target_link_libraries(epipolar ${OpenCV_LIBS})
