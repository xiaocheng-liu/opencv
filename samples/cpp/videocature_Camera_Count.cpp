#include <iostream>

#include "opencv2/videoio.hpp"

using namespace cv;
using namespace std;

int main() {
    int camera_count = 0;
    VideoCapture cap;

    // 假设最多检测 10 个摄像头
    for (int i = 0; i < 10; i++) {
        cap.open(i); // 尝试打开摄像头 i
        if (cap.isOpened()) {
            cout << "摄像头 " << i << " 已连接。" << endl;
            camera_count++;
            cap.release(); // 释放摄像头
        }
    }

    cout << "总共检测到 " << camera_count << " 个摄像头。" << endl;
    return 0;
}
