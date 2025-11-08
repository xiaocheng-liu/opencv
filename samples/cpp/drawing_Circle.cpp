#include "opencv2/core.hpp"      // 引入OpenCV核心模块
#include "opencv2/highgui.hpp"   // 引入OpenCV图形界面模块
#include "opencv2/imgproc.hpp"   // 引入OpenCV图像处理模块

using namespace cv;  // 使用OpenCV命名空间

/**
 * 生成一张随机彩色圆圈图。
 */
int main(int argc, char *argv[]) {
    // 创建一个400x400像素的黑色背景图像（3通道彩色图像，CV_8UC3表示8位无符号3通道）
    Mat img = Mat::zeros(400, 400, CV_8UC3);

    // 初始化随机数生成器，种子为12345（固定种子确保每次运行结果一致）
    RNG rng(12345);

    // 循环生成20个随机圆圈
    for (int i = 0; i < 20; i++) {
        // 随机生成圆心坐标（x和y在图像范围内随机）
        Point center(rng.uniform(0, img.cols), rng.uniform(0, img.rows));

        // 随机生成圆的半径（范围5到50像素）
        int radius = rng.uniform(5, 50);

        // 随机生成圆的颜色（RGB值范围0到255）
        Scalar color(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

        // 在图像上绘制圆圈
        // 参数说明：
        //   img: 目标图像
        //   center: 圆心坐标
        //   radius: 圆半径
        //   color: 圆的颜色（RGB值）
        //   FILLED: 填充模式（实心圆）
        circle(img, center, radius, color, FILLED);
    }

    // 显示生成的图像，窗口标题为"Random Circles"
    imshow("Random Circles", img);

    // 等待用户按键后关闭窗口
    waitKey();

    return 0;  // 程序正常退出
}
