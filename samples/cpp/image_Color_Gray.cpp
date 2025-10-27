#include <opencv2/core/core.hpp>
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

// ---------- 灰度化算法函数 ----------

// 平均值法
Mat grayAverage(const Mat& img) {
    Mat gray(img.rows, img.cols, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            Vec3b bgr = img.at<Vec3b>(y, x);
            uchar grayValue = (bgr[0] + bgr[1] + bgr[2]) / 3;
            gray.at<uchar>(y, x) = grayValue;
        }
    }
    return gray;
}

// 加权平均法（符合人眼感知）
Mat grayWeighted(const Mat& img) {
    Mat gray(img.rows, img.cols, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            Vec3b bgr = img.at<Vec3b>(y, x);
            uchar grayValue = static_cast<uchar>(
                0.114 * bgr[0] + 0.587 * bgr[1] + 0.299 * bgr[2]  // 注意BGR顺序
            );
            gray.at<uchar>(y, x) = grayValue;
        }
    }
    return gray;
}

// 最大值法
Mat grayMax(const Mat& img) {
    Mat gray(img.rows, img.cols, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            Vec3b bgr = img.at<Vec3b>(y, x);
            uchar grayValue = max(bgr[0], max(bgr[1], bgr[2]));
            gray.at<uchar>(y, x) = grayValue;
        }
    }
    return gray;
}

// 最小值法
Mat grayMin(const Mat& img) {
    Mat gray(img.rows, img.cols, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            Vec3b bgr = img.at<Vec3b>(y, x);
            uchar grayValue = min(bgr[0], min(bgr[1], bgr[2]));
            gray.at<uchar>(y, x) = grayValue;
        }
    }
    return gray;
}

// 自定义权重法
Mat grayCustom(const Mat& img, double wb, double wg, double wr) {
    Mat gray(img.rows, img.cols, CV_8UC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            Vec3b bgr = img.at<Vec3b>(y, x);
            uchar grayValue = static_cast<uchar>(
                wb * bgr[0] + wg * bgr[1] + wr * bgr[2]
            );
            gray.at<uchar>(y, x) = grayValue;
        }
    }
    return gray;
}

// ---------- 主函数 ----------
int main() {
    Mat img = imread(samples::findFile("dog.jpg"));
    if (img.empty()) {
        cout << "无法加载图像！" << endl;
        return -1;
    }

    Mat gray_avg = grayAverage(img);
    Mat gray_weighted = grayWeighted(img);
    Mat gray_max = grayMax(img);
    Mat gray_min = grayMin(img);
    Mat gray_custom = grayCustom(img, 0.2, 0.7, 0.1);

    // 使用 OpenCV 自带的转换作为对照
    Mat gray_cv;
    cvtColor(img, gray_cv, COLOR_BGR2GRAY);

    imshow("Original", img);
    imshow("Average Gray", gray_avg);
    imshow("Weighted Gray", gray_weighted);
    imshow("Max Gray", gray_max);
    imshow("Min Gray", gray_min);
    imshow("Custom Gray (0.2,0.7,0.1)", gray_custom);
    imshow("OpenCV cvtColor Gray", gray_cv);

    waitKey(0);
    destroyAllWindows();
    return 0;
}