#include <iostream>
#include <opencv2/core.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

int main() {
    Mat img = imread(samples::findFile("apple.jpg"));
    if (img.empty()) {
        cout << "无法读取图像!" << endl;
        return -1;
    }

    // 将 BGR 转换为 YCrCb（注意 OpenCV 用 CrCb 顺序）
    Mat ycrcb;
    cvtColor(img, ycrcb, COLOR_BGR2YCrCb);

    // 分离三个通道
    vector<Mat> channels;
    split(ycrcb, channels);
    Mat Y = channels[0];
    Mat Cr = channels[1];
    Mat Cb = channels[2];

    // 显示各通道（用灰度显示）
    imshow("Original Image", img);
    imshow("Y", Y);
    imshow("Cr", Cr);
    imshow("Cb", Cb);

    waitKey(0);
    return 0;
}