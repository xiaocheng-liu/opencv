#include <opencv2/core.hpp>
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/gapi/imgproc.hpp"

using namespace cv;
using namespace std;

/**
 * 调整图像的大小（缩放）
 */
int main() {
    Mat img = imread(samples::findFile("aero1.jpg"));
    Mat imgResize;

    resize(img, imgResize, Size(), 0.5, 0.5);
    imshow("Image", img);
    imshow("Image Resize", imgResize);
    waitKey(0);
    return 0;
}