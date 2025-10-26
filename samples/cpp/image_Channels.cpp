#include <iostream>
#include <opencv2/core.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;
using namespace std;

int main() {
    // 1️⃣ 读取一张彩色图像（默认BGR通道顺序）
    Mat img = imread(samples::findFile("lena.jpg"));
    if (img.empty()) {
        cout << "❌ 无法加载图像，请检查路径！" << endl;
        return -1;
    }

    // 2️⃣ 分离图像的三个通道：B、G、R
    vector<Mat> channels;
    split(img, channels); // 拆分到3个单通道Mat中
    // channels[0] -> Blue 通道
    // channels[1] -> Green 通道
    // channels[2] -> Red 通道

    // 3️⃣ 显示每个通道的灰度图像（单通道）
    imshow("Original Image", img);
    imshow("Blue Channel", channels[0]);
    imshow("Green Channel", channels[1]);
    imshow("Red Channel", channels[2]);

    // 4️⃣ 为了更直观地观察每个颜色分量，可将单通道合成为伪彩色图
    Mat blueColor, greenColor, redColor;
    Mat zero = Mat::zeros(img.size(), CV_8UC1); // 全黑通道

    // merge() 将三个通道重新组合为彩色图像
    merge(vector<Mat>{channels[0], zero, zero}, blueColor);  // 仅显示蓝色分量
    merge(vector<Mat>{zero, channels[1], zero}, greenColor); // 仅显示绿色分量
    merge(vector<Mat>{zero, zero, channels[2]}, redColor);   // 仅显示红色分量

    // 5️⃣ 显示结果
    imshow("Blue Component (Pseudo Color)", blueColor);
    imshow("Green Component (Pseudo Color)", greenColor);
    imshow("Red Component (Pseudo Color)", redColor);

    // 6️⃣ 等待用户按键退出
    waitKey(0);
    return 0;
}