#include <iostream>
#include <stdio.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String hand_cascade_name = "../haar_data/haar_fist.xml";
CascadeClassifier hand_cascade;
string window_name = "Capture - Hand detection";
RNG rng(12345);

/** @function main */
int main( int argc, const char** argv ) {
  Mat frame = imread(argv[1], -1);

  //-- 1. Load the cascades
  if( !hand_cascade.load( hand_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

  //-- 2. Read the video stream
  detectAndDisplay(frame);
  waitKey(0);
  
  return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame ) {
  std::vector<Rect> hand;
  Mat frame_gray;

  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  hand_cascade.detectMultiScale( frame_gray, hand, 1.01, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < hand.size(); i++ ) {
    Point center( hand[i].x + hand[i].width*0.5, hand[i].y + hand[i].height*0.5 );
    ellipse( frame, center, Size(hand[i].width*0.5, hand[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0);
  }
  //-- Show what you got
  imshow( window_name, frame );
}

