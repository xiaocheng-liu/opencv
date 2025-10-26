#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    string path = samples::findFile("vtest.avi");
    VideoCapture cap(path);

    Mat img;
    if (!cap.isOpened()) {
        cout << "无法打开视频文件！" << endl;
        return -1;
    }

    while (true) {
        cap.read(img);
        imshow("Video", img);
        waitKey(33);
    }
    return 0;
}