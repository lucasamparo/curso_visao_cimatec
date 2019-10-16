cmake_minimum_required(VERSION 3.1)
project(haar)

set(CMAKE_GXX_FLAGS "-Wall -Wextra -Wconversion  -pedantic -std=gnu11")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(face_detector src/haar_face_eyes.cpp)
target_link_libraries(face_detector ${OpenCV_LIBS})

add_executable(upperbody_detector src/haar_body.cpp)
target_link_libraries(upperbody_detector ${OpenCV_LIBS})

add_executable(hand_detector src/haar_hand.cpp)
target_link_libraries(hand_detector ${OpenCV_LIBS})
