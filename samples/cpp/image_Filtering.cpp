// 引入OpenCV核心模块，包含基本数据结构和算法
#include <opencv2/core.hpp>
// 引入标准输入输出库
#include <iostream>

// 引入OpenCV高级GUI模块，提供图像显示功能
#include "opencv2/highgui.hpp"
// 引入OpenCV图像编解码模块，支持多种图像格式的读写
#include "opencv2/imgcodecs.hpp"
// 引入OpenCV图像处理模块，提供滤波、边缘检测等算法
#include "opencv2/imgproc.hpp"

// 使用OpenCV命名空间，简化代码
using namespace cv;
// 使用标准命名空间，简化代码
using namespace std;

/**
 * 图像滤波示例程序
 * 功能：对输入图像进行四种不同类型的滤波处理，并显示结果
 */
int main() {
    // 读取图像文件
    // samples::findFile("apple.jpg")：查找OpenCV自带的测试图像"apple.jpg"
    // imread()：读取图像文件，返回Mat对象
    Mat src = imread(samples::findFile("apple.jpg"));

    // 检查图像是否成功加载
    if (src.empty()) {
        cout << "无法加载图像！" << endl;
        return -1; // 返回错误码-1
    }

    // 定义四个Mat对象，用于存储不同滤波方法的结果
    Mat blurImg;    // 均值滤波结果
    Mat gaussImg;   // 高斯滤波结果
    Mat medianImg;  // 中值滤波结果
    Mat bilateralImg; // 双边滤波结果

    // 对图像进行均值滤波
    // blur()：均值滤波函数
    // 参数说明：
    //   - src：输入图像
    //   - blurImg：输出图像
    //   - Size(3,3)：滤波核尺寸（3x3）
    blur(src, blurImg, Size(3, 3));

    // 对图像进行高斯滤波
    // GaussianBlur()：高斯滤波函数
    // 参数说明：
    //   - src：输入图像
    //   - gaussImg：输出图像
    //   - Size(5,5)：滤波核尺寸（5x5）
    //   - 1.5：高斯核的标准差
    GaussianBlur(src, gaussImg, Size(5, 5), 1.5);

    // 对图像进行中值滤波
    // medianBlur()：中值滤波函数
    // 参数说明：
    //   - src：输入图像
    //   - medianImg：输出图像
    //   - 3：滤波核尺寸（3x3）
    medianBlur(src, medianImg, 3);

    // 对图像进行双边滤波
    // bilateralFilter()：双边滤波函数
    // 参数说明：
    //   - src：输入图像
    //   - bilateralImg：输出图像
    //   - 9：滤波核直径
    //   - 75：颜色空间的标准差
    //   - 75：坐标空间的标准差
    bilateralFilter(src, bilateralImg, 9, 75, 75);

    // 显示原始图像
    imshow("Original", src);
    // 显示均值滤波结果
    imshow("Mean Filter", blurImg);
    // 显示高斯滤波结果
    imshow("Gaussian Filter", gaussImg);
    // 显示中值滤波结果
    imshow("Median Filter", medianImg);
    // 显示双边滤波结果
    imshow("Bilateral Filter", bilateralImg);

    // 等待用户按键
    // waitKey(0)：无限等待用户按键，按任意键后程序继续执行
    waitKey(0);

    // 程序正常退出
    return 0;
}
