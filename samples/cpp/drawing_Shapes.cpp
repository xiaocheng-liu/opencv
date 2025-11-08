#include "opencv2/core.hpp"      // 引入OpenCV核心模块，包含基本数据结构和算法
#include <iostream>              // 引入标准输入输出库

#include "opencv2/highgui.hpp"   // 引入OpenCV图形界面模块，用于图像显示
#include "opencv2/imgproc.hpp"   // 引入OpenCV图像处理模块，用于绘制图形
using namespace cv;              // 使用OpenCV命名空间
using namespace std;             // 使用标准命名空间

/**
 * 生成随机颜色（BGR格式）
 * @return Scalar对象，包含随机的B、G、R值（范围：0-255）
 */
Scalar getRandomColor() {
    // 直接生成随机数，不重复初始化种子
    //srand(static_cast<unsigned int>(time(nullptr)));

    // 生成随机的B、G、R值
    int b = rand() % 256;  // 蓝色分量
    int g = rand() % 256;  // 绿色分量
    int r = rand() % 256;  // 红色分量

    return Scalar(b, g, r);  // 返回随机颜色
}

/**
 * 主函数：绘制多种几何图形并显示
 */
int main() {
    // 创建一个512x512像素的白色背景图像（3通道彩色图像，CV_8UC3表示8位无符号3通道）
    Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));
    RNG rng(12345);


    // 在图像上绘制文字
    // 参数说明：
    //   img: 目标图像
    //   "Drawing Shapes": 要绘制的文本内容
    //   Point(128, 50): 文本左下角坐标
    //   FONT_HERSHEY_SIMPLEX: 字体类型
    //   1.0: 字体大小
    //   Scalar(0, 0, 0): 文本颜色（黑色）
    //   2: 线宽
    //   LINE_AA: 抗锯齿线型
    putText(img, "Drawing Shapes", Point(128, 50), FONT_HERSHEY_SIMPLEX, 1.0, getRandomColor(), 2, LINE_AA);

    // 绘制圆形
    // 参数说明：
    //   img: 目标图像
    //   Point(256, 256): 圆心坐标
    //   128: 半径
    //   Scalar(0, 0, 255): 颜色（红色）
    //   2: 线宽
    // 随机生成圆的颜色（RGB值范围0到255）
    circle(img, Point(256, 256), 128, getRandomColor(), 2);

    // 绘制直线
    // 参数说明：
    //   img: 目标图像
    //   Point(128, 128): 起点坐标
    //   Point(384, 384): 终点坐标
    //   Scalar(0, 255, 0): 颜色（绿色）
    //   2: 线宽
    line(img, Point(128, 128), Point(384, 384), getRandomColor(), 2);

    // 绘制矩形
    // 参数说明：
    //   img: 目标图像
    //   Point(128, 128): 矩形左上角坐标
    //   Point(384, 384): 矩形右下角坐标
    //   Scalar(255, 0, 0): 颜色（蓝色）
    //   2: 线宽
    rectangle(img, Point(128, 128), Point(384, 384), getRandomColor(), 2);

    // 绘制椭圆
    // 参数说明：
    //   img: 目标图像
    //   Point(256, 256): 椭圆中心坐标
    //   Size(128, 64): 椭圆长轴和短轴长度
    //   0: 旋转角度（0度）
    //   0, 360: 起始和结束角度（绘制完整椭圆）
    //   Scalar(0, 0, 255): 颜色（红色）
    //   2: 线宽
    ellipse(img, Point(256, 256), Size(128, 64), 0, 0, 360, getRandomColor(), 2);

    // 定义三角形的三个顶点
    // 参数说明：
    //   Point(x, y): 顶点坐标
    Point pts_3[3] = {
        Point(256, 256),  // 顶点1：中心偏上
        Point(128, 384),  // 顶点2：左下角
        Point(384, 384)   // 顶点3：右下角
    };

    // 绘制三角形的轮廓（不填充）
    // 参数说明：
    //   img: 目标图像
    //   ppt: 顶点数组的指针（需转换为const Point*类型）
    //   npt: 顶点数量数组（每个多边形的顶点数）
    //   1: 多边形数量（当前只绘制一个三角形）
    //   true: 是否闭合多边形（三角形必须闭合）
    //   Scalar(0, 0, 255): 轮廓颜色（红色，BGR格式）
    //   2: 线宽
    //   LINE_AA: 抗锯齿线型（使边缘更平滑）
    const Point* ppt_3[1] = {pts_3};  // 将顶点数组包装为指针数组
    int npt_3[] = {3};              // 指定顶点数量为3
    polylines(img, ppt_3, npt_3, 1, true, getRandomColor(), 2, LINE_AA);


    // 定义五边形的五个顶点
    // 参数说明：
    //   Point(x, y): 顶点坐标
    Point pts_5[5] = {
        Point(256, 100),  // 顶点1：顶部
        Point(150, 250),  // 顶点2：左上
        Point(180, 400),  // 顶点3：左下
        Point(332, 400),  // 顶点4：右下
        Point(362, 250)   // 顶点5：右上
    };

    // 绘制五边形的轮廓（不填充）
    // 参数说明：
    //   img: 目标图像
    //   ppt: 顶点数组的指针（需转换为const Point*类型）
    //   npt: 顶点数量数组（每个多边形的顶点数）
    //   1: 多边形数量（当前只绘制一个五边形）
    //   true: 是否闭合多边形（五边形必须闭合）
    //   Scalar(0, 0, 255): 轮廓颜色（红色，BGR格式）
    //   2: 线宽
    //   LINE_AA: 抗锯齿线型（使边缘更平滑）
    const Point* ppt_5[1] = {pts_5};  // 将顶点数组包装为指针数组
    int npt_5[] = {5};              // 指定顶点数量为5
    polylines(img, ppt_5, npt_5, 1, true, getRandomColor(), 2, LINE_AA);

    // 显示图像，窗口标题为"Drawing Shapes"
    imshow("Drawing Shapes", img);

    // 等待用户按键后关闭窗口
    waitKey(0);

    return 0;  // 程序正常退出
}
