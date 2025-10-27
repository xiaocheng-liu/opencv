#include "opencv2/core.hpp"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;

/**
 * 均值滤波的“线性平均”会把边缘当作噪声一起平滑掉。
 */
int main() {
    Mat img = imread(samples::findFile("lena.jpg"));
    if (img.empty()) return -1;

    Mat blur3, blur5, blur7, blur9;
    blur(img, blur3, Size(3,3));
    blur(img, blur5, Size(5,5));
    blur(img, blur7, Size(7, 7));
    blur(img, blur9, Size(9,9));

    imshow("Original", img);
    imshow("Blur 3x3", blur3);
    imshow("Blur 5x5", blur5);
    imshow("Blur 7x7", blur7);
    imshow("Blur 9x9", blur9);

    waitKey(0);
    return 0;
}