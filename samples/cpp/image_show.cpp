#include "opencv2/core.hpp"
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;
using namespace std;

/**
 * 显示图像
 */
int main(const int argc, const char* args[]) {
    // 读取图像
    Mat image = imread(samples::findFile("lena.jpg"), IMREAD_COLOR);

    // 检查是否读取成功
    if (image.empty()) {
        cout << "无法打开或找到图像文件！" << endl;
        return -1;
    }

    // 打印图像信息
    cout << "图像尺寸: " << image.rows << "x" << image.cols <<
        ", 通道数: " << image.channels() << endl;

    // 创建窗口并显示
    imshow("lena", image);

    // 等待按键（0表示无限等待)
    waitKey(0);

    // 关闭所有窗口
    destroyAllWindows();

    return 0;
}
