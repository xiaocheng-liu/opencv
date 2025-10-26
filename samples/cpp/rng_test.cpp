#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

/**
 * 生成一张随机彩色圆圈图。
 */
int main(int argc, char *argv[]) {
    Mat img = Mat::zeros(400, 400, CV_8UC3);
    RNG rng(12345);

    for (int i = 0; i < 20; i++) {
        Point center(rng.uniform(0, img.cols), rng.uniform(0, img.rows));
        int radius = rng.uniform(5, 50);
        Scalar color(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        circle(img, center, radius, color, FILLED);
    }

    imshow("Random Circles", img);
    waitKey();
}