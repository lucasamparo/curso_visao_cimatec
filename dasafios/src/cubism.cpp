#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

cv::Scalar median(cv::Mat src) {
  double red = 0, blue = 0, green = 0;
  for(int i = 0; i < src.cols; i++) {
    for(int j = 0; j < src.rows; j++) {
      cv::Vec3b color = src.at<cv::Vec3b>(i, j);
      blue += color[0];
      green += color[1];
      red += color[2];
    }
  }
  
  double size = src.cols * src.rows;  
  return cv::Scalar(ceil(blue/size), ceil(green/size), ceil(red/size));
}

int random(int max_v, int min_v = 0) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min_v, max_v);

  return dist(rng);
}

cv::Mat rotateBlock(cv::Mat src, double angle) {
  cv::Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
  cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);

  cv::Mat dst;
  cv::warpAffine(src, dst, rot, src.size());
  
  return dst;
}

int main(int argc, char ** argv) {
  cv::Mat original_image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  
  // Cube size
  int min_c = 2;
  int max_c = 6;
  
  cv::Mat zeros = cv::Mat::zeros(original_image.size(), CV_8U);
  cv::Mat final_image = original_image.clone();
  //cv::Mat final_image =cv::Mat::zeros(original_image.size(), CV_8UC3);
  
  int n_samples = 20000, count = 0;
  while(count < n_samples) {
    int local_c = random(max_c, min_c);
    int x = std::max(random(original_image.cols), local_c);
    int y = std::max(random(original_image.rows), local_c);
    
    cv::Rect roi = cv::Rect(
                      x, y,
                      std::min(local_c*2 +1, original_image.cols - x),
                      std::min(local_c*2 +1, original_image.rows - y)
                    );
    
    cv::Mat mask = zeros.clone();
    mask(roi) = 255;
    cv::Mat tmp;
    original_image.copyTo(tmp, mask);
    
    tmp(roi) = median(tmp(roi));
    
    cv::Mat rotated = rotateBlock(tmp, random(10, -10));
    
    for(int i = 0; i < final_image.cols; i++) {
      for(int j = 0; j < final_image.rows; j++) {
        cv::Vec3b v1 = final_image.at<cv::Vec3b>(i, j);
        cv::Vec3b v2 = rotated.at<cv::Vec3b>(i, j);
        
        cv::Vec3b z = cv::Vec3b(0, 0, 0);
        
        if(v1 == z and v2 == z) {
          final_image.at<cv::Vec3b>(i, j) = z;
        } else if(v1 == z) {
          final_image.at<cv::Vec3b>(i, j) = v2;
        } else if(v2 == z) {
          final_image.at<cv::Vec3b>(i, j) = v1;
        } else {
          int b = (v1[0] + v2[0]) / 2.0;
          int g = (v1[1] + v2[1]) / 2.0;
          int r = (v1[2] + v2[2]) / 2.0;
          final_image.at<cv::Vec3b>(i, j) = cv::Vec3b(b, g, r);
        }
      }
    }
    count++;
  }
  
  cv::Mat presentation;
  cv::hconcat(original_image, final_image, presentation);
  
  cv::imshow("image", presentation);
  cv::imwrite("cubism.png", presentation);
  cv::waitKey(0);
}
