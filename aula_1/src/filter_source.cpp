#include "filtering/filter_source.hpp"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "iostream"

FilterSource::FilterSource() {

}

void FilterSource::cartoonCallback(int state, void* userData) {

    FilterSource* filter_object = (FilterSource*) userData;

    /** EDGES **/
    // Apply median filter to remove possible noise
    cv::Mat imgMedian;
    cv::medianBlur(filter_object->original_image, imgMedian, 7);

    // Detect edges with canny
    cv::Mat imgCanny;
    cv::Canny(imgMedian, imgCanny, 50, 150);

    // Dilate the edges
    cv::Mat kernel= getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
    cv::dilate(imgCanny, imgCanny, kernel);

    // Scale edges values to 1 and invert values
    imgCanny= imgCanny/255;
    imgCanny= 1-imgCanny;

    // Use float values to allow multiply between 0 and 1
    cv::Mat imgCannyf;
    imgCanny.convertTo(imgCannyf, CV_32FC3);

    // Blur the edgest to do smooth effect
    cv::blur(imgCannyf, imgCannyf, cv::Size(5,5));

    /** COLOR **/
    // Apply bilateral filter to homogenizes color
    cv::Mat imgBF;
    cv::bilateralFilter(filter_object->original_image, imgBF, 9, 150.0, 150.0);

    // truncate colors
    cv::Mat result= imgBF/25;
    result = result*25;

    /** MERGES COLOR + EDGES **/
    // Create a 3 channles for edges
    cv::Mat imgCanny3c;
    cv::Mat cannyChannels[]={ imgCannyf, imgCannyf, imgCannyf};
    cv::merge(cannyChannels, 3, imgCanny3c);

    // Convert color result to float 
    cv::Mat resultf;
    result.convertTo(resultf, CV_32FC3);

    // Multiply color and edges matrices
    cv::multiply(resultf, imgCanny3c, resultf);

    // convert to 8 bits color
    resultf.convertTo(result, CV_8UC3);

    // Show image
    cv::imshow("Result", result);

}

void FilterSource::rcColorSpace(int state, void* userData) {

    FilterSource* filter_object = (FilterSource*) userData;

    std::vector<cv::Mat> spl;
    cv::Mat result;

    /* Simulate conversion from BGR to RC (red, cyan).
    The source and destination images must both be in BGR format.
    Blues and greens are replaced with cyans.
    Pseudocode:
    dst.b = dst.g = 0.5 * (src.b + src.g)
    dst.r = src.r
    */
    cv::split(filter_object->original_image, spl);
    cv::addWeighted(spl[0], 0.5, spl[1], 0.5, 0, spl[0]);
    spl.at(1) = spl.at(0);
    cv::merge(spl.data(), 3, result);

    // Show image
    cv::imshow("Result", result);

}

void FilterSource::strokeEdges(int state, void* userData) {

    FilterSource* filter_object = (FilterSource*) userData;

    cv::Mat bluerred;
    cv::Mat gray_scale;
    cv::Mat normalized_inverse_alpha;

    std::vector<cv::Mat> spl;

    int k_size = 5;

    cv::medianBlur(filter_object->original_image, bluerred, k_size);
    cv::cvtColor(bluerred, gray_scale, cv::COLOR_BGR2GRAY);
    cv::Laplacian(gray_scale, gray_scale, CV_8U, k_size);

    normalized_inverse_alpha = (1.0 / 255) * (255 - gray_scale);

    cv::split(filter_object->original_image, spl);

    for (cv::Mat& channel : spl) {
        channel = channel.mul(normalized_inverse_alpha);
    }
    
    cv::Mat result;
    cv::merge(spl.data(), 3, result);

    cv::imshow("Result", result);

}