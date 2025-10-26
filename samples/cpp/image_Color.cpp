#include <iostream>
#include <opencv2/core.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

int main() {
    // 1️⃣ 读取一张彩色图像（默认BGR格式）
    Mat img = imread(samples::findFile("lena.jpg"));
    if (img.empty()) {
        cout << "无法加载图像！请检查路径。" << endl;
        return -1;
    }

    // 2️⃣ 定义若干目标 Mat，用于存储不同颜色空间的图像
    Mat gray, hsv, lab, ycrcb;

    // 3️⃣ 颜色空间转换函数：cvtColor()
    // 参数：输入图像、输出图像、转换类型
    cvtColor(img, gray, COLOR_BGR2GRAY);   // BGR → 灰度
    cvtColor(img, hsv, COLOR_BGR2HSV);     // BGR → HSV（色调、饱和度、亮度）
    cvtColor(img, lab, COLOR_BGR2Lab);     // BGR → Lab（亮度 + 对比度更稳定）
    cvtColor(img, ycrcb, COLOR_BGR2YCrCb); // BGR → YCrCb（常用于视频压缩）

    // 4️⃣ 显示所有结果
    imshow("Original (BGR)", img);
    imshow("Grayscale", gray);
    imshow("HSV", hsv);
    imshow("Lab", lab);
    imshow("YCrCb", ycrcb);

    // 5️⃣ 等待用户按键退出
    waitKey(0);
    return 0;
}