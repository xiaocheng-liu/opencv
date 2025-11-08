#include <opencv2/core.hpp>       // 引入OpenCV核心模块
#include <iostream>              // 引入标准输入输出库

#include "opencv2/imgcodecs.hpp"  // 引入OpenCV图像编解码模块
using namespace cv;              // 使用OpenCV命名空间
using namespace std;             // 使用标准命名空间

int main(int argc, char** argv) {
    // 读取图像文件
    // samples::findFile("lena.jpg")：查找名为"lena.jpg"的测试图像文件路径
    // imread：读取图像文件，返回一个Mat对象（多通道图像）
    Mat img = imread((samples::findFile("lena.jpg")));

    // 定义一个无符号字符的动态数组，用于存储编码后的图像数据
    vector<uchar> buf;
    // 定义一个整型动态数组，用于存储编码参数
    vector<int> params;

    // 设置JPEG编码参数：质量为50（范围：0-100，值越高质量越好）
    params = {IMWRITE_JPEG_QUALITY, 50};
    // 将图像编码为JPEG格式，并存储到buf中
    // imencode(".jpg", img, buf, params)：编码函数
    //   - ".jpg"：指定输出格式为JPEG
    //   - img：输入图像
    //   - buf：输出缓冲区
    //   - params：编码参数
    imencode(".jpg", img, buf, params);
    // 输出JPEG图像的大小（单位为KB）
    cout << "JPEG(50) size: " << buf.size() / 1024 << " KB\n";

    // 设置PNG编码参数：压缩级别为3（范围：0-9，值越高压缩率越高）
    params = {IMWRITE_PNG_COMPRESSION, 3};
    // 将图像编码为PNG格式，并存储到buf中
    imencode(".png", img, buf, params);
    // 输出PNG图像的大小（单位为KB）
    cout << "PNG size: " << buf.size() / 1024 << " KB\n";

    // 将图像编码为BMP格式（BMP是无损格式，无压缩参数）
    imencode(".bmp", img, buf);
    // 输出BMP图像的大小（单位为KB）
    cout << "BMP size: " << buf.size() / 1024 << " KB\n";

    return 0;  // 程序正常退出
}
