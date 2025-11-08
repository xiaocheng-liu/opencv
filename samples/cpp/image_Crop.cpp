#include <opencv2/core.hpp>       // 引入OpenCV核心模块，包含基本数据结构和算法
#include <iostream>              // 引入标准输入输出库，用于控制台输出

#include "opencv2/highgui.hpp"    // 引入OpenCV高级GUI模块，提供图像显示功能
#include "opencv2/imgcodecs.hpp"  // 引入OpenCV图像编解码模块，支持多种图像格式的读写
#include "opencv2/gapi/imgproc.hpp" // 引入OpenCV G-API模块，提供图像处理功能（虽然本示例未直接使用）

using namespace cv;              // 使用OpenCV命名空间，简化代码
using namespace std;             // 使用标准命名空间，简化代码

/**
 * 图片剪切示例程序
 * 功能：读取一张图片，剪切指定区域并显示原始图片和剪切后的图片
 */
int main() {
    // 读取图片文件
    // samples::findFile("lena.jpg")：OpenCV提供的工具函数，用于查找测试图片"lena.jpg"的路径
    // imread()：读取图片文件，返回一个Mat对象（多通道图像矩阵）
    Mat img = imread(samples::findFile("lena.jpg"));

    // 定义感兴趣区域(ROI)
    // Rect(x, y, width, height)：定义一个矩形区域
    // 参数说明：
    //   - x: 矩形左上角的x坐标（128）
    //   - y: 矩形左上角的y坐标（230）
    //   - width: 矩形的宽度（280）
    //   - height: 矩形的高度（256）
    Rect roi(128, 230, 280, 256);

    // 剪切图片
    // img(roi)：通过ROI剪切图片，返回剪切后的图像矩阵
    Mat imgCrop = img(roi);

    // 显示原始图片
    // imshow()：在窗口中显示图片
    // 参数说明：
    //   - "Image"：窗口标题
    //   - img：要显示的图片
    imshow("Image", img);

    // 显示剪切后的图片
    imshow("Image Crop", imgCrop);

    // 等待用户按键
    // waitKey(0)：无限等待用户按键，按任意键后程序继续执行
    waitKey(0);

    return 0;  // 程序正常退出
}
