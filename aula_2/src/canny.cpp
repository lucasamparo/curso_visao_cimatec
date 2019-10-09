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
  int kernel_size = stoi(argv[2]);
  int threshold = stoi(argv[3]);
  
  // Load OpenCV image withou change anything on their color pattern
  Mat image = imread(path, cv::IMREAD_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;
  
  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  
  // Apply Canny edge detection
  Mat output;
  blur(gray, gray, Size(kernel_size, kernel_size));
  Canny(gray, output, threshold, 3*threshold, kernel_size );
  
  Mat final_output = Mat::zeros(rows, cols, image.type());
  image.copyTo(final_output, output);
  
  showImage(unifyImages({image, final_output}, 1));
  
  return 0;
}
