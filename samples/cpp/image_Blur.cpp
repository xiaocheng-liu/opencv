// 引入OpenCV核心模块，包含基本数据结构和算法
#include "opencv2/core.hpp"
// 引入OpenCV高级GUI模块，提供图像显示功能
#include "opencv2/highgui.hpp"
// 引入OpenCV图像处理模块，提供滤波、边缘检测等算法
#include "opencv2/imgproc.hpp"
// 使用OpenCV命名空间，简化代码
using namespace cv;

/**
 * 均值滤波示例程序
 * 功能：对输入图像进行不同尺寸的均值滤波处理，并显示结果
 * 说明：均值滤波是一种线性平滑滤波方法，会模糊图像边缘
 */
int main() {
    // 读取图像文件
    // samples::findFile("lena.jpg")：查找OpenCV自带的测试图像"lena.jpg"
    // imread()：读取图像文件，返回Mat对象
    Mat img = imread(samples::findFile("lena.jpg"));
    
    // 检查图像是否成功加载
    if (img.empty()) {
        // 图像加载失败，返回错误码-1
        return -1;
    }

    // 定义四个Mat对象，用于存储不同尺寸的滤波结果
    Mat blur3, blur5, blur7, blur9;

    // 对图像进行3x3均值滤波
    // blur()：均值滤波函数
    // 参数说明：
    //   - img：输入图像
    //   - blur3：输出图像
    //   - Size(3,3)：滤波核尺寸（3x3）
    blur(img, blur3, Size(3,3));

    // 对图像进行5x5均值滤波
    blur(img, blur5, Size(5,5));

    // 对图像进行7x7均值滤波
    blur(img, blur7, Size(7,7));

    // 对图像进行9x9均值滤波
    blur(img, blur9, Size(9,9));

    // 显示原始图像
    // imshow()：在窗口中显示图像
    // 参数说明：
    //   - "Original"：窗口标题
    //   - img：要显示的图像
    imshow("Original", img);

    // 显示3x3滤波结果
    imshow("Blur 3x3", blur3);

    // 显示5x5滤波结果
    imshow("Blur 5x5", blur5);

    // 显示7x7滤波结果
    imshow("Blur 7x7", blur7);

    // 显示9x9滤波结果
    imshow("Blur 9x9", blur9);

    // 等待用户按键
    // waitKey(0)：无限等待用户按键，按任意键后程序继续执行
    waitKey(0);

    // 程序正常退出
    return 0;
}
