#include <opencv2/core.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;

int main() {
    // 1️⃣ 创建一张空白画布（300行 × 600列，3通道图像）
    // CV_8UC3 表示：8位无符号整型，3个颜色通道（B, G, R）
    Mat canvas(300, 600, CV_8UC3);

    // 2️⃣ 通过遍历像素来生成颜色混合效果
    // 横轴 (x) 控制 红色分量（R）
    // 纵轴 (y) 控制 绿色分量（G）
    // 蓝色分量 (B) 固定为 128（中等亮度的蓝色）
    for (int y = 0; y < canvas.rows; y++) {
        for (int x = 0; x < canvas.cols; x++) {
            // 计算当前像素的 R、G、B 分量
            uchar r = static_cast<uchar>(x * 255 / canvas.cols); // 从左到右，红色逐渐增强
            uchar g = static_cast<uchar>(y * 255 / canvas.rows); // 从上到下，绿色逐渐增强
            uchar b = 128; // 蓝色保持中等亮度（可以改成 0 或 255 观察差异）

            // 设置像素值（注意 OpenCV 默认顺序是 BGR，不是 RGB）
            canvas.at<Vec3b>(y, x) = Vec3b(b, g, r);
        }
    }

    // 3️⃣ 在图像上绘制说明文字
    putText(canvas, "R increases ->", Point(200, 20),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
    putText(canvas, "G increases downwards", Point(10, 290),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);

    // 4️⃣ 显示结果窗口
    imshow("RGB Color Mixing Demo", canvas);

    // 5️⃣ 等待用户按任意键退出
    waitKey(0);
    return 0;
}