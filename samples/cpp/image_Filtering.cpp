#include <opencv2/core/core.hpp>
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

int main() {
    Mat src = imread(samples::findFile("apple.jpg"));
    if (src.empty()) {
        cout << "无法加载图像！" << endl;
        return -1;
    }

    Mat blurImg, gaussImg, medianImg, bilateralImg;

    blur(src, blurImg, Size(3, 3));                             // 均值滤波
    GaussianBlur(src, gaussImg, Size(5, 5), 1.5);       // 高斯滤波
    medianBlur(src, medianImg, 3);                                          // 中值滤波
    bilateralFilter(src, bilateralImg, 9, 75, 75);          // 双边滤波

    imshow("Original", src);
    imshow("Mean Filter", blurImg);
    imshow("Gaussian Filter", gaussImg);
    imshow("Median Filter", medianImg);
    imshow("Bilateral Filter", bilateralImg);

    waitKey(0);
    return 0;
}
