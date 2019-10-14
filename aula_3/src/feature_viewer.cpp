#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

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

Mat siftKeypoints(Mat image) {
  Ptr<Feature2D> detector = xfeatures2d::SiftFeatureDetector::create();
  vector<KeyPoint> keypoints;
  detector->detect(image, keypoints);

  Mat output;
  drawKeypoints(image, keypoints, output);

  return output;
}

Mat orbKeypoints(Mat image, int max_keypoints) {
  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);

  std::vector<KeyPoint> keypoints;
  Mat descriptors;

  Ptr<Feature2D> orb = ORB::create(max_keypoints);
  orb->detectAndCompute(gray, Mat(), keypoints, descriptors);

  Mat output;
  drawKeypoints(image, keypoints, output);

  return output;
}

int main(int argc, char ** argv) {
  string path = argv[1];
  int keypoint_type = 0;
  int max_keypoints = 500;
  
  // Load OpenCV image withou change anything on their color pattern
  Mat image = imread(path, cv::IMREAD_UNCHANGED);
  
  // Get image dimensions
  int cols = image.cols;
  int rows = image.rows;

  Mat feature_image;

  switch(keypoint_type) {
    case 0:
      feature_image = orbKeypoints(image, max_keypoints);
    break;
    case 1:
      //feature_image = siftKeypoints(image);
    break;
    case 2:
      // feature_image = surfKeypoints(image);
    break;
  }

  showImage(unifyImages({image, feature_image}, 1));
  
  return 0;
}
