#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// OpenCV includes
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

int main( int argc, const char** argv )
{
	// Read images
	cv::Mat color= cv::imread("../samples/corrosion.png");
	cv::Mat gray= cv::imread("../samples/corrosion.png", cv::IMREAD_GRAYSCALE);
	
	if(! color.data ) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl ;
	 	return -1;
	}

	// Write images
	cv::imwrite("corrosion.jpg", gray);
	
	// Get same pixel with opencv function
	int myRow=0;
	int myCol=0;
	auto pixel= color.at<cv::Vec3b>(myRow, myCol);
	cout << "Pixel value (B,G,R): (" << (int)pixel[0] << "," << (int)pixel[1] << "," << (int)pixel[2] << ")" << endl;
	
	// show images
	cv::imshow("Corrosion BGR", color);
	cv::imshow("Corrosion Gray", gray);
	// wait for any key press
	cv::waitKey(0);
	return 0;
}