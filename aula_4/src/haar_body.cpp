#include <iostream>
#include <stdio.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String body_cascade_name = "../haar_data/haar_upperbody.xml";
CascadeClassifier body_cascade;
string window_name = "Capture - Upper Body detection";
RNG rng(12345);

/** @function main */
int main( int argc, const char** argv ) {
  Mat frame = imread(argv[1], -1);

  //-- 1. Load the cascades
  if( !body_cascade.load( body_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

  //-- 2. Read the video stream
  detectAndDisplay(frame);
  waitKey(0);
  
  return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame ) {
  std::vector<Rect> body;
  Mat frame_gray;

  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  body_cascade.detectMultiScale( frame_gray, body, 1.01, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < body.size(); i++ ) {
    Point center( body[i].x + body[i].width*0.5, body[i].y + body[i].height*0.5 );
    ellipse( frame, center, Size(body[i].width*0.5, body[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0);
  }
  //-- Show what you got
  imshow( window_name, frame );
}

