#include <opencv2/core.hpp>
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/gapi/imgproc.hpp"

using namespace cv;
using namespace std;

/**
 * 图片剪切
 */
int main() {

    Mat img = imread(samples::findFile("lena.jpg"));
    Rect roi(128, 230, 280, 256);
    Mat imgCrop = img(roi);

    imshow("Image", img);
    imshow("Image Crop", imgCrop);
    waitKey(0);
    return 0;
}