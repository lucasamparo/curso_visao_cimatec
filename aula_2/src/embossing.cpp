#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void showImage(Mat image) {
  // Present imagem on the screen
  imshow("image", image);
  waitKey(0);
}

Mat unifyImages(vector<Mat> images, int orientation) {
  // Put images with same pattern on one unique image
  Mat output;
  images[0].copyTo(output);
  switch(orientation) {
    case 0:
    // Vertical
      for(int i = 1; i < images.size(); i++) {
        vconcat(output, images[i], output);
      }
    break;
    case 1:
    // Horizontal
      for(int i = 1; i < images.size(); i++) {
        hconcat(output, images[i], output);
      }
    break;
  }
  
  return output;
}

int main(int argc, char ** argv) {
  string path = argv[1];
  int kernel_size = 3;
  
  // Load OpenCV image withou change anything on their color pattern
  Mat image = imread(path, cv::IMREAD_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;
  
  // Apply embossing filter
  Mat output, gray;
  Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
  kernel.at<float>(0, 0) = -1;
  kernel.at<float>(0, 1) = -1;
  kernel.at<float>(1, 0) = -1;
  
  kernel.at<float>(1, 2) = 1;
  kernel.at<float>(2, 1) = 1;
  kernel.at<float>(2, 2) = 1;
  
  // Converting color image to grayscale image
  cvtColor(image, gray, COLOR_BGR2GRAY);  
  
  filter2D(gray, output, -1, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
  
  // Converting back to color pattern to show together
  Mat gray_color, output_color;
  cvtColor(gray, gray_color, COLOR_GRAY2BGR);
  cvtColor(output, output_color, COLOR_GRAY2BGR);
  
  showImage(unifyImages({image, gray_color, output_color}, 1));
  
  return 0;
}
