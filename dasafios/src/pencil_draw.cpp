#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
  cv::Mat original_image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  
  cv::Mat image;
  cvtColor(original_image, image, cv::COLOR_BGR2GRAY);
  
  cv::Mat negative = 255 - image;
  
  cv::Mat blur;
  GaussianBlur(negative, blur, cv::Size(5, 5), 0, 0 );
  
  cv::Mat base_draw;
  cv::divide(image, 255 - blur, base_draw, 256);
  
  cv::Mat final_image;
  cv::cvtColor(base_draw, final_image, cv::COLOR_GRAY2BGR);
  
  cv::Mat presentation;
  cv::hconcat(original_image, final_image, presentation);
  
  cv::imshow("image", presentation);
  cv::imwrite("pencil_draw.png", presentation);
  cv::waitKey(0);
}
