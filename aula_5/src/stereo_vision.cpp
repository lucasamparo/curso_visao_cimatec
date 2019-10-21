/**
 * @file SBM_Sample
 * @brief Get a disparity map of two images
 * @author A. Huaman
 */
#include <iostream>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

const char *windowDisparity = "Disparity";

/**
 * @function main
 * @brief Main function
 */
int main(int argc, char ** argv) {
  //-- 1. Read the images
  Mat imgLeft = imread( argv[1], IMREAD_GRAYSCALE );
  Mat imgRight = imread( argv[2], IMREAD_GRAYSCALE );
  
  //-- And create the image in which we will save our disparities
  Mat imgDisparity16S = Mat( imgLeft.rows, imgLeft.cols, CV_16S );
  Mat imgDisparity8U = Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );

  if( imgLeft.empty() || imgRight.empty() ) {
    std::cout<< " --(!) Error reading images " << std::endl;
    return -1;
  }

  //-- 2. Call the constructor for StereoBM
  int ndisparities = 16;   /**< Range of disparity */
  int SADWindowSize = 15; /**< Size of the block window. Must be odd */

  Ptr<StereoBM> sbm = StereoBM::create( ndisparities, SADWindowSize );

  //-- 3. Calculate the disparity image
  sbm->compute( imgLeft, imgRight, imgDisparity16S );

  //-- Check its extreme values
  double minVal;
  double maxVal;

  minMaxLoc( imgDisparity16S, &minVal, &maxVal );

  printf("Min disp: %f Max value: %f \n", minVal, maxVal);

  //-- 4. Display it as a CV_8UC1 image
  imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));

  namedWindow( windowDisparity, WINDOW_NORMAL );
  imshow( windowDisparity, imgDisparity8U );

  waitKey(0);

  return 0;
}
