#include <opencv2/core.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;

/**
* 与均值滤波相同，高斯滤波的计算过程也是：
    1.	用高斯核覆盖图像局部；
    2.	对应像素相乘；
    3.	求和；
    4.	替换中心像素值。

    不同的是：
    → 权重来自高斯分布，而不是平均权重。
 */
int main() {
    Mat img = imread(samples::findFile("lena.jpg"));
    if (img.empty()) return -1;

    Mat dst1, dst2, dst3;
    GaussianBlur(img, dst1, Size(3,3), 0);
    GaussianBlur(img, dst2, Size(5,5), 1.5);
    GaussianBlur(img, dst3, Size(9,9), 2.5);

    imshow("Original", img);
    imshow("Gaussian 3x3", dst1);
    imshow("Gaussian 5x5", dst2);
    imshow("Gaussian 9x9", dst3);

    waitKey(0);
    return 0;
}