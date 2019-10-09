cmake_minimum_required (VERSION 3.5)
project(reading_writing_files)
set (CMAKE_CXX_STANDARD 11)
find_package( OpenCV 4.0.0 REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS} include)

ADD_EXECUTABLE( read_writting_image src/read_writting_image.cpp )
TARGET_LINK_LIBRARIES( read_writting_image ${OpenCV_LIBS} )

add_library(filter_source src/filter_source.cpp)
target_link_libraries(filter_source ${OpenCV_LIBS})

ADD_EXECUTABLE( read_writting_video src/read_writting_video.cpp )
TARGET_LINK_LIBRARIES( read_writting_video ${OpenCV_LIBS} filter_source)