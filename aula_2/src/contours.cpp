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
  
  // Convert to grayscale
  Mat gray;
  cvtColor(image, gray, CV_BGR2GRAY);
  
  // Canny detector
  Mat canny;
  Canny(gray, canny, 50, 100, 3);
  
  // Contours
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
  
  // Draw Contours
  Mat draw = Mat::zeros(canny.size(), CV_8U);
  for( int i = 0; i< contours.size(); i++ ) {
    Scalar color = Scalar(255);
    drawContours(draw, contours, i, color, 2, 8, hierarchy, 0, Point());
    
    // Contour features
    cout << "Contour n " << (i + 1) << endl;
    cout << "Area: " << contourArea(contours[i]) << endl;
    cout << "Perimeter: " << arcLength(contours[i], true) << endl;
    cout << "Convex? " << ((isContourConvex(contours[i])) ? "Yes" : "No") << endl;
    cout << endl;
  }
  
  // Converting back to BGR to show together
  Mat gray_color, draw_color;
  cvtColor(gray, gray_color, CV_GRAY2BGR);
  cvtColor(draw, draw_color, CV_GRAY2BGR);
  
  showImage(unifyImages({image, gray_color, draw_color}, 1));
  
  return 0;
}
