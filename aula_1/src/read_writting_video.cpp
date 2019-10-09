#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include "filtering/filter_source.hpp"

using namespace std;

// OpenCV includes
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"

const char* keys =
{
	"{help h usage ? | | print this message}"
    "{@image | | Image to process}"
};



int main( int argc, const char** argv ) {

    FilterSource filters;

    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("help"))	{
	    parser.printMessage();
	    return 0;
	}

    if (!parser.check())
	{
	    parser.printErrors();
	    return 0;
	}

    cv::String video_file= parser.get<cv::String>("@image");

    parser.about("Primeira aula: Manipulando videos e filtrando");

    cv::Mat frame;
    cv::Mat gray_scale;
    auto video_capture = cv::VideoCapture(video_file); // ../samples/sample.mp4
    auto fps = video_capture.get(cv::CAP_PROP_FPS);
    cv::Size size = cv::Size(int(video_capture.get(cv::CAP_PROP_FRAME_WIDTH)),
    int(video_capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
    auto video_writer = cv::VideoWriter(
        "MyOutputVid.mp4",  cv::VideoWriter::fourcc('X','2','6','4'), fps, size);

    bool success = video_capture.read(frame);

    cv::Mat frame0 = frame;

    while(success) {
        // Loop until there are no more frames.
        cv::cvtColor(frame, gray_scale, cv::COLOR_BGR2GRAY);
        cv::cvtColor(gray_scale, gray_scale, cv::COLOR_GRAY2BGR);
        video_writer.write(gray_scale);
        success = video_capture.read(frame);
    }

    video_capture.release();
    video_writer.release();

    cv::imshow("Result", frame0);

    filters.original_image = frame0;

    cv::namedWindow("Input");

    cv::createButton("Cartoonizing",
        &FilterSource::cartoonCallback,
        &filters, cv::QT_PUSH_BUTTON, 0);

    cv::createButton("RC Space",
        &FilterSource::rcColorSpace,
        &filters, cv::QT_PUSH_BUTTON, 0);

    cv::createButton("Defining edges",
        &FilterSource::strokeEdges,
        &filters, cv::QT_PUSH_BUTTON, 0);

    cv::waitKey(0);
	return 0;
    
}

