#include <opencv2/core/core.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

/**
 * Harris 角点检测（最经典）
 */
int main() {
    Mat img = imread(samples::findFile("feature_test.jpg"), IMREAD_GRAYSCALE);
    if (img.empty()) return -1;

    Mat dst, dst_norm;
    dst = Mat::zeros(img.size(), CV_32FC1);

    // Harris 角点检测
    cornerHarris(img, dst, 2, 3, 0.04);

    // 归一化到 0~255
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1);

    Mat dst_norm_scaled;
    convertScaleAbs(dst_norm, dst_norm_scaled);

    // 标记角点
    for (int j = 0; j < dst_norm.rows; j++) {
        for (int i = 0; i < dst_norm.cols; i++) {
            if ((int)dst_norm.at<float>(j,i) > 150) { // 阈值可调
                circle(dst_norm_scaled, Point(i,j), 5, Scalar(0,0,255), 2);
            }
        }
    }

    imshow("Harris Corners", dst_norm_scaled);
    waitKey(0);
    return 0;
}