#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
  cv::Mat original_image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  cv::Mat puzzle_pattern = cv::imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
  
  cv::Mat resized_pattern, thin_pattern;
  cv::resize(puzzle_pattern, resized_pattern, cv::Size(original_image.cols, original_image.rows));
  cv::threshold(resized_pattern, resized_pattern, 250, 255, 1);
  cv::erode(resized_pattern, thin_pattern, cv::Mat::ones(3, 3, CV_8U));

  double ele = M_PI/2.2, azi = M_PI/4.0, dep = 50.0;

  cv::Mat puzzle_image, gray, t_gray;
  original_image.copyTo(puzzle_image, 255 - resized_pattern);
  cvtColor(puzzle_image, gray, cv::COLOR_BGR2GRAY);
  
  //Gradients
  cv::Mat grad_x, grad_y;
  Sobel(gray, grad_x, CV_32F, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
  Sobel(gray, grad_y, CV_32F, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);

  // simulating rays
  double gd = std::cos(ele);
  double dx = gd*std::cos(azi);
  double dy = gd*std::sin(azi);
  double dz = std::sin(ele);

  // Unit vectors
  cv::Mat length, grad_x2, grad_y2, uni_x, uni_y, uni_z;
  cv::pow(grad_x, 2, grad_x2);
  cv::pow(grad_y, 2, grad_y2);
  cv::sqrt((grad_x2 + grad_y2 + 1), length);
  cv::divide(grad_x, length, uni_x);
  cv::divide(grad_y, length, uni_y);
  cv::divide(cv::Mat::ones(grad_x.size(), CV_32F), length, uni_x);

  // Relief
  cv::Mat embossed = 255*(dx*uni_x + dy*uni_y + dz*uni_z);

  cv::Mat embossed_filter, with_lines;
  embossed.copyTo(embossed_filter, resized_pattern);

  cv::Mat embossed_final;
  float data[9] = {0, 2, 0, 0, 0, 0, 0, 0, 0};
  cv::Mat kernel = cv::Mat(3, 3, CV_32F, data);
  cv::filter2D(embossed_filter, embossed_final, -1, kernel);
  
  cv::Mat final_image, color_gray, original_32f;
  original_image.convertTo(original_32f, CV_32FC3);
  cv::cvtColor(embossed_final, color_gray, cv::COLOR_GRAY2BGR);
  cv::add(original_32f, color_gray, original_32f);

  original_32f.convertTo(final_image, CV_8UC3);
  
  cv::imshow("image", final_image);
  cv::waitKey(0);
}
