#include "opencv2/core.hpp"
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

/**
 * 绘制图形
 */
int main() {

    Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));
    // 绘制文字
    putText(img, "Drawing Shapes", Point(128, 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2, LINE_AA);

    // 绘制圆
    circle(img, Point(256, 256), 128, Scalar(0, 0, 255), 2);
    // 绘制线
    line(img, Point(128, 128), Point(384, 384), Scalar(0, 255, 0), 2);
    // 绘制矩形
    rectangle(img, Point(128, 128), Point(384, 384), Scalar(255, 0, 0), 2);
    // 绘制椭圆
    ellipse(img, Point(256, 256), Size(128, 64), 0, 0, 360, Scalar(0, 0, 255), 2);

    // 绘制三角形
    // 定义三角形的三个顶点
    Point pts[3] = {Point(256, 100), Point(100, 400), Point(400, 400)};

    // 绘制三角形
    const Point* ppt[1] = {pts};
    int npt[] = {3};
    polylines(img, ppt, npt, 1, true, Scalar(0, 0, 0), 2, LINE_AA); // 红色三角形

    imshow("Drawing Shapes", img);
    waitKey(0);

    return 0;
}