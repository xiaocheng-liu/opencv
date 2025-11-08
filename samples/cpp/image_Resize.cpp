// 引入OpenCV核心模块，包含基本数据结构和算法
#include <opencv2/core.hpp>
// 引入标准输入输出库，用于控制台输出
#include <iostream>

// 引入OpenCV高级GUI模块，提供图像显示功能
#include "opencv2/highgui.hpp"
// 引入OpenCV图像编解码模块，支持多种图像格式的读写
#include "opencv2/imgcodecs.hpp"
// 引入OpenCV G-API模块，提供图像处理功能（虽然本示例未直接使用）
#include "opencv2/gapi/imgproc.hpp"

// 使用OpenCV命名空间，简化代码
using namespace cv;
// 使用标准命名空间，简化代码
using namespace std;

/**
 * 图像缩放示例程序
 * 功能：读取一张图片，将其缩小为原始尺寸的一半并显示
 */
int main() {
    // 读取图片文件
    // samples::findFile("aero1.jpg")：OpenCV提供的工具函数，用于查找测试图片"aero1.jpg"的路径
    // imread()：读取图片文件，返回一个Mat对象（多通道图像矩阵）
    Mat img = imread(samples::findFile("aero1.jpg"));

    // 检查图片是否成功加载
    if (img.empty()) {
        cerr << "错误：无法加载图片文件！" << endl;
        return -1; // 返回错误码-1
    }

    // 定义缩放后的图片矩阵
    Mat imgResize;

    // 缩放图片
    // resize()：图像缩放函数
    // 参数说明：
    //   - img：输入图像
    //   - imgResize：输出图像
    //   - Size()：目标尺寸（空表示使用缩放因子）
    //   - 0.5：水平方向的缩放因子（缩小为50%）
    //   - 0.5：垂直方向的缩放因子（缩小为50%）
    resize(img, imgResize, Size(), 0.5, 0.5);

    // 显示原始图片
    // imshow()：在窗口中显示图片
    // 参数说明：
    //   - "Image"：窗口标题
    //   - img：要显示的图片
    imshow("Image", img);

    // 显示缩放后的图片
    imshow("Image Resize", imgResize);

    // 等待用户按键
    // waitKey(0)：无限等待用户按键，按任意键后程序继续执行
    waitKey(0);

    // 程序正常退出
    return 0;
}
