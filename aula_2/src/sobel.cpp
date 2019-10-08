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
  
  // Load OpenCV image withou change anything on their color pattern
  Mat image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;
  
  // Convert BGR to Gray
  Mat gray, output;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  
  // Apply Sobel
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  // Gradient in X axis
  Sobel(gray, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs(grad_x, abs_grad_x);

  // Gradient in Y axis
  Sobel(gray, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs(grad_y, abs_grad_y);

  /// Total Gradient (approximate)
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
  
  // Converting back to color pattern to show together
  Mat gray_color, output_color;
  cvtColor(gray, gray_color, COLOR_GRAY2BGR);
  cvtColor(output, output_color, COLOR_GRAY2BGR);
  
  Mat partA = unifyImages({image, gray_color, output_color}, 1);
  Mat partB = unifyImages({gray, abs_grad_x, abs_grad_y}, 1);
  
  Mat partB_color;
  cvtColor(partB, partB_color, COLOR_GRAY2BGR);
  
  showImage(unifyImages({partA, partB_color}, 0));
  
  return 0;
}
