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
  
  // Load OpenCV image withou change anything on their color pattern
  Mat image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;
  
  // Apply motion blur filter
  Mat output;
  Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
  kernel.row((kernel_size-1)/2) = 1;
  kernel = kernel / kernel_size;
  filter2D(image, output, -1, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
  
  showImage(unifyImages({image, output}, 1));
  
  return 0;
}
