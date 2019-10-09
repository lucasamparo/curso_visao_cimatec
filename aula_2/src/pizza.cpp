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
  Mat image = imread(path, cv::IMREAD_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;
  
  // Convert to grayscale
  Mat gray;
  cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  
  // Canny detector
  Mat canny;
  Canny(gray, canny, 50, 100, 3);
  
  // Contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(canny, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));
  
  // Draw Contours
  Mat draw = Mat::zeros(canny.size(), CV_8UC3);
  for( int i = 0; i< contours.size(); i++ ) {    
    // Refine contour
    float e = 0.01 * arcLength(contours[i], true);
    Mat new_contour;
    approxPolyDP(contours[i], new_contour, e, true);
    
    // Make a fitting on the new contour
    Mat hull;
    vector<Vec4i> defects;
    convexHull(new_contour, hull, false, false);
    convexityDefects(new_contour, hull, defects);
    
    // Evaluate the defects on the convexity of the contour
    if(defects.size() == 0)
      continue;
      
    // If a defect are found, show
    for(const Vec4i& v : defects) {
      Scalar color = Scalar(255, 255, 255);
      drawContours(draw, contours, i, color, 2, 8, hierarchy, 0, Point());
    }
  }
  
  showImage(unifyImages({image, draw}, 1));
  
  return 0;
}
