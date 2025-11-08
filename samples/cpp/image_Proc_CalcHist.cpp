#include <opencv2/core.hpp>
#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    string imgPath = samples::findFile("lena.jpg");
    Mat src = imread(imgPath);
    if (src.empty()) {
        cerr << "Cannot open image: " << imgPath << "\n";
        return -1;
    }
    imshow("Src", src);

    // ———— 1. 灰度直方图 ————
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    int histSize = 256;              // bin 数
    float range[] = { 0, 256 };      // 值域
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    Mat histGray;

    calcHist(&gray, 1, 0, Mat(), histGray, 1, &histSize, &histRange, uniform, accumulate);

    // 归一化，以便绘制
    int hist_h = 400;
    int hist_w = 512;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(histGray, histGray, 0, histImage.rows, NORM_MINMAX);

    for (int i = 1; i < histSize; i++) {
        line(histImage,
             Point(bin_w * (i - 1), hist_h - cvRound(histGray.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(histGray.at<float>(i))),
             Scalar(255, 255, 255), 2, 8);
    }
    imshow("Gray Histogram", histImage);

    // ———— 2. 彩色通道直方图（B、G、R） ————
    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX);
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX);
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX);

    Mat histImageColor(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    for (int i = 1; i < histSize; i++) {
        line(histImageColor,
             Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
             Scalar(255, 0, 0), 2, 8);
        line(histImageColor,
             Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
             Scalar(0, 255, 0), 2, 8);
        line(histImageColor,
             Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
             Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
             Scalar(0, 0, 255), 2, 8);
    }

    imshow("Color Histogram", histImageColor);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
