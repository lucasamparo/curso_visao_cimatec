#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

cv::Point maxValue(cv::Mat src) {
  double minVal; 
  double maxVal; 
  cv::Point minLoc; 
  cv::Point maxLoc;

  cv::minMaxLoc(src, &minVal, &maxVal, &minLoc, &maxLoc);
  
  return maxLoc;
}

int main(int argc, char ** argv) {
  cv::Mat original_image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  cv::Mat final_image = cv::Mat(original_image.size(), CV_8UC3);
  
  int levels = 20, brushSize = 5;
  int height = original_image.rows, width = original_image.cols;
  
  for(int y = brushSize; y < (height - brushSize); y++) {
    for(int x = brushSize; x < (width - brushSize); x++) {
      std::vector<int> intensity(levels, 0), redmean(levels, 0),
          bluemean(levels, 0), greenmean(levels, 0);
      for(int j = -brushSize; j <= brushSize; j++) {
        for(int i = -brushSize; i <= brushSize; i++) {
          cv::Vec3b color = original_image.at<cv::Vec3b>(y+j, x+i);
          
          double b = color[0];
          double g = color[1];
          double r = color[2];
          
          double gray = (r + g + b) / 3.0;
          
          int inner_intensity = (gray * levels) / 255;
          
          int tmp = inner_intensity;
          intensity[tmp]++;
          redmean[tmp] += r;
          bluemean[tmp] += b;
          greenmean[tmp] += g;
        }
      }
      
      int maxId = std::max_element(std::begin(intensity), std::end(intensity)) - intensity.begin();
      int maxValue = intensity[maxId];
      
      double tmpB = bluemean[maxId] / maxValue;
      double tmpG = greenmean[maxId] / maxValue;
      double tmpR = redmean[maxId] / maxValue;
      
      cv::Vec3b newcolor(tmpB, tmpG, tmpR);
      
      final_image.at<cv::Vec3b>(y, x) = newcolor;
    }
  }
  
  cv::Mat presentation;
  cv::hconcat(original_image, final_image, presentation);
  
  cv::imshow("result", presentation);
  cv::imwrite("oilpaint.png", presentation);
  cv::waitKey(0);
}
