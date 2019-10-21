#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
  cv::Mat image1 = cv::imread(argv[1], 0);
  cv::Mat image2 = cv::imread(argv[2], 0);

  // vector of keypoints
  std::vector<cv::KeyPoint> keypoints1;
  std::vector<cv::KeyPoint> keypoints2;

  // Construction of the ORB feature detector
  cv::Ptr<cv::Feature2D> orb = cv::ORB::create();

  // Detection of the SURF features
  orb->detect(image1, keypoints1);
  orb->detect(image2, keypoints2);

  std::cout << "Number of ORB points (1): " << keypoints1.size() << std::endl;
  std::cout << "Number of ORB points (2): " << keypoints2.size() << std::endl;

  // Draw the kepoints
  cv::Mat imageKP;
  cv::drawKeypoints(image1, keypoints1, imageKP, cv::Scalar(255, 255, 255),
                    cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  cv::namedWindow("Right ORB Features");
  cv::imshow("Right ORB Features", imageKP);
  cv::drawKeypoints(image2, keypoints2, imageKP, cv::Scalar(255, 255, 255),
                    cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  cv::namedWindow("Left ORB Features");
  cv::imshow("Left ORB Features", imageKP);

  // Extraction of the SURF descriptors
  cv::Mat descriptors1, descriptors2;
  orb->compute(image1, keypoints1, descriptors1);
  orb->compute(image2, keypoints2, descriptors2);

  std::cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;

  // Construction of the matcher
  auto matcher = cv::BFMatcher::create();

  // Match the two image descriptors
  std::vector<cv::DMatch> matches;
  matcher->match(descriptors1, descriptors2, matches);

  std::cout << "Number of matched points: " << matches.size() << std::endl;

  // Select few Matches
  std::vector<cv::DMatch> selMatches;
  selMatches.push_back(matches[14]);
  selMatches.push_back(matches[16]);
  selMatches.push_back(matches[141]);
  selMatches.push_back(matches[146]);
  selMatches.push_back(matches[235]);
  selMatches.push_back(matches[238]);
  selMatches.push_back(matches[274]);

  // Draw the selected matches
  cv::Mat imageMatches;
  cv::drawMatches(
      image1, keypoints1, // 1st image and its keypoints
      image2, keypoints2, // 2nd image and its keypoints
      matches,                    // the matches
      imageMatches,               // the image produced
      cv::Scalar(255, 255, 255)); // color of the lines
  cv::namedWindow("Matches");
  cv::imshow("Matches", imageMatches);

  // Convert 1 vector of keypoints into
  // 2 vectors of Point2f
  std::vector<int> pointIndexes1;
  std::vector<int> pointIndexes2;
  for (auto it = selMatches.begin(); it != selMatches.end(); ++it) {
      pointIndexes1.push_back(it->queryIdx);
      pointIndexes2.push_back(it->trainIdx);
  }

  // Convert keypoints into Point2f
  std::vector<cv::Point2f> selPoints1, selPoints2;
  cv::KeyPoint::convert(keypoints1, selPoints1, pointIndexes1);
  cv::KeyPoint::convert(keypoints2, selPoints2, pointIndexes2);

  // check by drawing the points
  auto it = selPoints1.begin();
  while (it != selPoints1.end()) {
      cv::circle(image1, *it, 3, cv::Scalar(255, 255, 255), 2);
      ++it;
  }

  it = selPoints2.begin();
  while (it != selPoints2.end()) {
      cv::circle(image2, *it, 3, cv::Scalar(255, 255, 255), 2);
      ++it;
  }

  // Compute F matrix from 7 matches
  cv::Mat fundemental = cv::findFundamentalMat(cv::Mat(selPoints1), // points in first image
                                               cv::Mat(selPoints2), // points in second image
                                               cv::FM_7POINT);

  std::cout << "F-Matrix size= " << fundemental.rows << "," << fundemental.cols << std::endl;

  cv::Rect roi(0, 0, 3, 3);
  cv::Mat fund_part(fundemental, roi);

  std::cout << "F-Matrix size= " << fund_part.rows << "," << fund_part.cols << std::endl;

  // draw the left points corresponding epipolar lines in right image
  std::vector<cv::Vec3f> lines1;
  cv::computeCorrespondEpilines(cv::Mat(selPoints1), // image points
                                1,                   // in image 1 (can also be 2)
                                fund_part,         // F matrix
                                lines1);             // vector of epipolar lines

  // for all epipolar lines
  for (auto it = lines1.begin(); it != lines1.end(); ++it) {
      cv::line(image2, cv::Point(0, -(*it)[2] / (*it)[1]),
               cv::Point(image2.cols, -((*it)[2] + (*it)[0] * image2.cols) / (*it)[1]),
               cv::Scalar(255, 255, 255));
  }

  // draw the left points corresponding epipolar lines in left image
  std::vector<cv::Vec3f> lines2;
  cv::computeCorrespondEpilines(cv::Mat(selPoints2), 2, fund_part, lines2);
  for (auto it = lines2.begin(); it != lines2.end(); ++it) {
      cv::line(image1, cv::Point(0, -(*it)[2] / (*it)[1]),
               cv::Point(image1.cols, -((*it)[2] + (*it)[0] * image1.cols) / (*it)[1]),
               cv::Scalar(255, 255, 255));
  }

  // Display the images with points and epipolar lines
  cv::namedWindow("Right Image Epilines");
  cv::imshow("Right Image Epilines", image1);
  cv::namedWindow("Left Image Epilines");
  cv::imshow("Left Image Epilines", image2);

  cv::waitKey(0);

  return 0;
}
