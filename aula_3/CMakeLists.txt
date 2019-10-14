cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

find_package(OpenCV 3)

set (CMAKE_CXX_STANDARD 11)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(features src/feature_viewer.cpp)
target_link_libraries(features ${OpenCV_LIBS})

add_executable(stitch src/image_stitch.cpp)
target_link_libraries(stitch ${OpenCV_LIBS})
