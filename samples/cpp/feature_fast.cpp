#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;
using namespace std;

/**
 * FAST 角点检测（速度快）
 */
int main() {
    Mat img = imread(samples::findFile("feature_test.jpg"), IMREAD_GRAYSCALE);
    if (img.empty()) return -1;

    vector<KeyPoint> keypoints;
    Ptr<FastFeatureDetector> fast = FastFeatureDetector::create(50, true); // 阈值50，非极大抑制
    fast->detect(img, keypoints);

    Mat out;
    drawKeypoints(img, keypoints, out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    imshow("FAST Keypoints", out);
    waitKey(0);
    return 0;
}