#include <opencv2/core.hpp>
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/gapi/imgproc.hpp"

using namespace cv;
using namespace std;

int main() {
    // 1. 读取图像
    Mat img = imread(samples::findFile("test.jpg"));
    if (img.empty()) {
        cout << "无法打开图像文件！" << endl;
        return -1;
    }

    // 显示原图
    imshow("Original", img);

    // =====================================================
    // 方法 1：卷积核锐化 (Kernel Sharpening)
    // =====================================================
    Mat kernel = (Mat_<float>(3,3) <<
         0, -1, 0,
        -1, 5, -1,
         0, -1, 0);
    Mat sharp1;
    gapi::filter2D(img, sharp1, -1, kernel);
    imshow("Sharpening - Kernel", sharp1);

    // =====================================================
    // 方法 2：拉普拉斯锐化 (Laplacian Sharpening)
    // =====================================================
    Mat gray, lap, sharp2;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    Laplacian(gray, lap, CV_16S, 3);
    convertScaleAbs(lap, lap);  // 转换回 8 位
    sharp2 = gray - lap;
    imshow("Sharpening - Laplacian", sharp2);

    // =====================================================
    // 方法 3：反锐化掩模 (Unsharp Mask)
    // =====================================================
    Mat blurred, mask, sharp3;
    GaussianBlur(img, blurred, Size(5,5), 1.5);
    mask = img - blurred;
    sharp3 = img + 1.0 * mask;  // 调整 1.0 改变锐化强度
    imshow("Sharpening - Unsharp Mask", sharp3);

    waitKey(0);
    destroyAllWindows();
    return 0;
}