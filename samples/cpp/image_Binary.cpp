#include <opencv2/core/core.hpp>
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

int main() {
    Mat src = imread(samples::findFile("dog.jpg"), IMREAD_GRAYSCALE);
    if (src.empty()) {
        cout << "无法加载图像！" << endl;
        return -1;
    }

    Mat binary_fixed, binary_inv, binary_otsu, binary_adapt;

    // 固定阈值
    threshold(src, binary_fixed, 128, 255, THRESH_BINARY);
    // 反向二值化
    threshold(src, binary_inv, 128, 255, THRESH_BINARY_INV);
    // Otsu 自动阈值
    threshold(src, binary_otsu, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // 自适应阈值
    adaptiveThreshold(src, binary_adapt, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
                      THRESH_BINARY, 11, 2);

    imshow("Original Gray", src);
    imshow("Fixed Threshold", binary_fixed);
    imshow("Inverse Threshold", binary_inv);
    imshow("Otsu Threshold", binary_otsu);
    imshow("Adaptive Threshold", binary_adapt);

    waitKey(0);
    destroyAllWindows();
    return 0;
}