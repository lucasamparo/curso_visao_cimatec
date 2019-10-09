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
  
  Mat output, image_yuv;
  
  // Convert to YUV Patter
  cvtColor(image, image_yuv, COLOR_BGR2YUV);
  
  // Split channels from image
  vector<Mat> yuv(3);
  split(image_yuv, yuv);
  
  // Aplly historgram equializer on Y channel
  Mat eq_y;
  equalizeHist(yuv[0], eq_y);
  yuv[0] = eq_y;
  
  // Merge image again
  cv::merge(yuv, image_yuv);
  
  // Reconvert to BGR
  cvtColor(image_yuv, output, COLOR_YUV2BGR);
  
  showImage(unifyImages({image, output}, 1));
  
  return 0;
}
