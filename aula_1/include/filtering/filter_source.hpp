#pragma once

#include "opencv2/core.hpp"

class FilterSource {
 public:
   
   FilterSource();
   static void cartoonCallback(int state, void* userData);
   static void rcColorSpace(int state, void* userData);
   static void strokeEdges(int state, void* userData);


 public:
    cv::Mat filtered_image;
    cv::Mat original_image;
    

};