find_package(OpenCV 4)

set (CMAKE_CXX_STANDARD 11)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(blur src/blur.cpp)
target_link_libraries(blur ${OpenCV_LIBRARIES})

#add_executable(canny src/canny.cpp)
#target_link_libraries(canny ${OpenCV_LIBRARIES})

#add_executable(color_segmentation src/color_segmentation.cpp)
#target_link_libraries(color_segmentation ${OpenCV_LIBRARIES})

#add_executable(contours src/contours.cpp)
#target_link_libraries(contours ${OpenCV_LIBRARIES})

#add_executable(contrast src/contrast.cpp)
#target_link_libraries(contrast ${OpenCV_LIBRARIES})

#add_executable(embossing src/embossing.cpp)
#target_link_libraries(embossing ${OpenCV_LIBRARIES})

#add_executable(motion_blur src/motion_blur.cpp)
#target_link_libraries(motion_blur ${OpenCV_LIBRARIES})

#add_executable(erosion_dilation src/erosion_dilation.cpp)
#target_link_libraries(erosion_dilation ${OpenCV_LIBRARIES})

#add_executable(pizza src/pizza.cpp)
#target_link_libraries(pizza ${OpenCV_LIBRARIES})

#add_executable(sharpening src/sharpening.cpp)
#target_link_libraries(sharpening ${OpenCV_LIBRARIES})

#add_executable(sobel src/sobel.cpp)
#target_link_libraries(sobel ${OpenCV_LIBRARIES})
