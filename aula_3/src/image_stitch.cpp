#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Stitcher::Mode mode = Stitcher::PANORAMA;
vector<Mat> imgs;

void showImage(Mat image) {
  // Present imagem on the screen
  imshow("image", image);
  waitKey(0);
}

int parseCmdArgs(int argc, char** argv) {
  if (argc == 1) {
      return EXIT_FAILURE;
  }
  for (int i = 1; i < argc; ++i) {
      Mat img = imread(argv[i]);
      if (img.empty()) {
          cout << "Can't read image '" << argv[i] << "'\n";
          return EXIT_FAILURE;
      }
      imgs.push_back(img);
  }
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
  int retval = parseCmdArgs(argc, argv);
  if (retval) return EXIT_FAILURE;
  Mat pano;
  Ptr<Stitcher> stitcher = Stitcher::create(mode);
  Stitcher::Status status = stitcher->stitch(imgs, pano);
  if (status != Stitcher::OK) {
      cout << "Can't stitch images, error code = " << int(status) << endl;
      return EXIT_FAILURE;
  }

  showImage(pano);

  return EXIT_SUCCESS;
}