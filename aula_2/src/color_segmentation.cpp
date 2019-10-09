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

Mat applyThreshold(Mat input, int value) {
  Mat ret;
  
  return ret;
}

int main(int argc, char ** argv) {
  string path = argv[1];
  
  // Load OpenCV image withou change anything on their color pattern
  Mat image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;
  
  // Create mask to avoid the white and black places on the image
  Mat gray, mask_white, mask_black;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, mask_white, 250, 255, 1);
  threshold(gray, mask_black, 10, 255, 0);
  
  Mat proc_image;
  bitwise_and(image, image, image, mask_black);
  bitwise_and(image, image, proc_image, mask_white);
  
  // Split channels from image
  vector<Mat> bgr(3);
  split(proc_image, bgr);
  
  // Segmenting based on the channel intensity
  // Blue above 100
  threshold(bgr[0], bgr[0], 100, 255, 0);
  // Green above 150
  threshold(bgr[1], bgr[1], 150, 255, 0);
  // Red above 200
  threshold(bgr[2], bgr[2], 200, 255, 0);
  
  Mat color_b, color_g, color_r;
  bitwise_and(image, image, color_b, bgr[0]);
  bitwise_and(image, image, color_g, bgr[1]);
  bitwise_and(image, image, color_r, bgr[2]);

  Mat partA = unifyImages({image, color_b}, 1);
  Mat partB = unifyImages({color_g, color_r}, 1);
  
  showImage(unifyImages({partA, partB}, 0));
  
  return 0;
}
