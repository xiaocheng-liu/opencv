#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>          // ORB/FAST
#include <opencv2/xfeatures2d.hpp>         // SIFT（需 opencv-contrib）
#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace cv::xfeatures2d;          // SIFT 命名空间
using namespace std;

int main() {
    // 1️⃣ 读取测试图片（灰度）
    Mat img = imread(samples::findFile("feature_test.jpg"), IMREAD_GRAYSCALE);
    if (img.empty()) {
        cerr << "图片读取失败，请检查路径" << endl;
        return -1;
    }

    // =======================
    // 2️⃣ Harris 角点检测
    // =======================
    Mat dst, dst_norm;
    dst = Mat::zeros(img.size(), CV_32FC1);  // 创建浮点图像存储角点响应
    int blockSize = 2;    // 邻域大小
    int ksize = 3;        // Sobel 卷积核大小
    double k = 0.04;      // Harris 参数，通常 0.04~0.06
    cornerHarris(img, dst, blockSize, ksize, k);

    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1); // 归一化到 0~255
    Mat dst_norm_scaled;
    convertScaleAbs(dst_norm, dst_norm_scaled);              // 转换为 8 位显示

    // 标记角点（阈值 150，可调）
    for (int j = 0; j < dst_norm.rows; j++) {
        for (int i = 0; i < dst_norm.cols; i++) {
            if ((int)dst_norm.at<float>(j,i) > 150) {
                circle(dst_norm_scaled, Point(i,j), 5, Scalar(0,0,255), 2);
            }
        }
    }

    imshow("Harris Corners", dst_norm_scaled);

    // =======================
    // 3️⃣ FAST 特征点检测
    // =======================
    vector<KeyPoint> fast_keypoints;
    Ptr<FastFeatureDetector> fast = FastFeatureDetector::create(
        50,   // 阈值，像素差异大于该值认为是角点
        true  // 是否使用非极大抑制
    );
    fast->detect(img, fast_keypoints);

    Mat fast_out;
    drawKeypoints(img, fast_keypoints, fast_out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("FAST Keypoints", fast_out);

    // =======================
    // 4️⃣ ORB 特征点检测
    // =======================
    Ptr<ORB> orb = ORB::create(500); // 最大检测 500 个关键点
    vector<KeyPoint> orb_keypoints;
    Mat orb_descriptors;
    orb->detectAndCompute(img, noArray(), orb_keypoints, orb_descriptors);

    Mat orb_out;
    drawKeypoints(img, orb_keypoints, orb_out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("ORB Keypoints", orb_out);

    // =======================
    // 5️⃣ SIFT 特征点检测（需 opencv-contrib）
    // =======================
    Ptr<SIFT> sift = SIFT::create();
    vector<KeyPoint> sift_keypoints;
    Mat sift_descriptors;
    sift->detectAndCompute(img, noArray(), sift_keypoints, sift_descriptors);

    Mat sift_out;
    drawKeypoints(img, sift_keypoints, sift_out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("SIFT Keypoints", sift_out);

    cout << "Harris: 未生成描述子，仅角点数量可视化" << endl;
    cout << "FAST: " << fast_keypoints.size() << " 个关键点" << endl;
    cout << "ORB: " << orb_keypoints.size() << " 个关键点" << endl;
    cout << "SIFT: " << sift_keypoints.size() << " 个关键点" << endl;

    waitKey(0);
    return 0;
}