// jpeg_visualize.cpp
// compile (example):
// g++ -std=c++17 jpeg_visualize.cpp `pkg-config --cflags --libs opencv4` -O2 -o jpeg_visualize
//
// usage:
// ./jpeg_visualize input.jpg [quality]
// quality: 1..100 (default 75)

#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

// Standard JPEG luminance quantization table
static const int STD_LUMA_Q[8][8] = {
    {16,11,10,16,24,40,51,61},
    {12,12,14,19,26,58,60,55},
    {14,13,16,24,40,57,69,56},
    {14,17,22,29,51,87,80,62},
    {18,22,37,56,68,109,103,77},
    {24,35,55,64,81,104,113,92},
    {49,64,78,87,103,121,120,101},
    {72,92,95,98,112,100,103,99}
};

// Build quant table scaled by quality parameter (1-100) as JPEG does
Mat buildQuantTable(int quality) {
    Mat q(8,8,CV_32F);
    int Q = quality;
    if (Q < 1) Q = 1;
    if (Q > 100) Q = 100;
    int scale;
    if (Q < 50) scale = 5000 / Q;
    else scale = 200 - 2 * Q;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            int v = STD_LUMA_Q[i][j];
            int qs = (v * scale + 50) / 100;
            if (qs < 1) qs = 1;
            if (qs > 255) qs = 255;
            q.at<float>(i,j) = (float)qs;
        }
    }
    return q;
}

// pad image to multiple of 8 by reflect border
Mat padTo8(const Mat &m) {
    int h = m.rows, w = m.cols;
    int nh = ((h + 7) / 8) * 8;
    int nw = ((w + 7) / 8) * 8;
    Mat dst;
    copyMakeBorder(m, dst, 0, nh - h, 0, nw - w, BORDER_REPLICATE);
    return dst;
}

// clip float Mat to uchar 0..255
Mat floatToU8(const Mat &f) {
    Mat out;
    Mat tmp;
    f.convertTo(tmp, CV_32F);
    out = Mat::zeros(tmp.size(), CV_8U);
    for (int y=0;y<tmp.rows;y++){
        for (int x=0;x<tmp.cols;x++){
            float v = tmp.at<float>(y,x);
            int iv = (int)round(v);
            if (iv < 0) iv = 0;
            if (iv > 255) iv = 255;
            out.at<uchar>(y,x) = (uchar)iv;
        }
    }
    return out;
}

int main(int argc, char** argv) {
    string inPath = samples::findFile("lena.jpg");
    int quality = 20;
    quality = max(1, min(100, quality));

    Mat img = imread(inPath, IMREAD_COLOR);
    if (img.empty()) {
        cerr << "Cannot open " << inPath << "\n";
        return -1;
    }

    // Convert to YCrCb and split
    Mat img_ycrcb;
    cvtColor(img, img_ycrcb, COLOR_BGR2YCrCb);
    vector<Mat> channels;
    split(img_ycrcb, channels); // channels[0]=Y, [1]=Cr, [2]=Cb

    Mat Y = channels[0];
    Mat Cr = channels[1];
    Mat Cb = channels[2];

    // pad to multiple of 8
    Mat Ypad = padTo8(Y);
    Mat originalYpad;
    Ypad.convertTo(originalYpad, CV_32F);

    int H = Ypad.rows, W = Ypad.cols;

    // build quant table
    Mat Q = buildQuantTable(quality);

    // prepare output Mats
    Mat dctMagnitude = Mat::zeros(H, W, CV_32F);
    Mat quantizedMag = Mat::zeros(H, W, CV_32F);
    Mat reconY = Mat::zeros(H, W, CV_32F);

    // process each 8x8 block
    for (int by=0; by<H; by+=8) {
        for (int bx=0; bx<W; bx+=8) {
            Mat block = Mat::zeros(8,8,CV_32F);
            // load block and shift by -128 (JPEG centers values)
            for (int i=0;i<8;i++){
                for (int j=0;j<8;j++){
                    float v = originalYpad.at<float>(by+i, bx+j);
                    block.at<float>(i,j) = v - 128.0f;
                }
            }
            Mat coeff;
            dct(block, coeff); // 8x8 DCT (type II)
            // store DCT magnitude (log scale) for visualization
            Mat abscoeff;
            absdiff(coeff, Scalar::all(0), abscoeff);
            for (int i=0;i<8;i++){
                for (int j=0;j<8;j++){
                    float val = abscoeff.at<float>(i,j);
                    dctMagnitude.at<float>(by+i, bx+j) = val;
                }
            }
            // quantize: round(coeff / Q)
            Mat qcoeff = Mat::zeros(8,8,CV_32F);
            for (int i=0;i<8;i++){
                for (int j=0;j<8;j++){
                    float v = coeff.at<float>(i,j);
                    float qv = Q.at<float>(i,j);
                    float qc = round(v / qv);
                    qcoeff.at<float>(i,j) = qc;
                    // store quantized magnitude for visualization
                    quantizedMag.at<float>(by+i, bx+j) = fabs(qc);
                }
            }
            // dequantize
            Mat deq = Mat::zeros(8,8,CV_32F);
            for (int i=0;i<8;i++){
                for (int j=0;j<8;j++){
                    deq.at<float>(i,j) = qcoeff.at<float>(i,j) * Q.at<float>(i,j);
                }
            }
            Mat idctblock;
            idct(deq, idctblock); // inverse DCT
            // add 128 back, write to reconY
            for (int i=0;i<8;i++){
                for (int j=0;j<8;j++){
                    float v = idctblock.at<float>(i,j) + 128.0f;
                    reconY.at<float>(by+i, bx+j) = v;
                }
            }
        }
    }

    // crop reconY to original size
    Mat reconYcrop = reconY(Rect(0,0, Y.cols, Y.rows));
    Mat reconY8u = floatToU8(reconYcrop);

    // merge with original Cr/Cb and convert back to BGR
    Mat outYCrCb;
    vector<Mat> outChannels;
    outChannels.push_back(reconY8u);
    outChannels.push_back(Cr);
    outChannels.push_back(Cb);
    merge(outChannels, outYCrCb);
    Mat reconBGR;
    cvtColor(outYCrCb, reconBGR, COLOR_YCrCb2BGR);

    // compute difference image
    Mat diff;
    absdiff(img, reconBGR, diff);

    // visualize DCT magnitude: log scale and normalize
    Mat dctVis;
    Mat dctCrop = dctMagnitude(Rect(0,0,Y.cols,Y.rows));
    Mat logDCT;
    dctCrop += 1e-6f; // avoid log(0)
    log(dctCrop, logDCT);
    normalize(logDCT, dctVis, 0, 255, NORM_MINMAX);
    dctVis.convertTo(dctVis, CV_8U);

    // visualize quantized coefficients magnitude
    Mat qVis;
    Mat qCrop = quantizedMag(Rect(0,0,Y.cols,Y.rows));
    Mat logQ;
    qCrop += 1e-6f;
    log(qCrop, logQ);
    normalize(logQ, qVis, 0, 255, NORM_MINMAX);
    qVis.convertTo(qVis, CV_8U);

    // also make a side-by-side comparison for one 8x8 block (top-left)
    int bx=0, by=0;
    Mat blockOrig = Mat::zeros(8,8,CV_8U);
    Mat blockDCTVis = Mat::zeros(8,8,CV_8U);
    Mat blockQVis = Mat::zeros(8,8,CV_8U);
    {
        Mat block = Mat::zeros(8,8,CV_32F);
        for (int i=0;i<8;i++) for (int j=0;j<8;j++) block.at<float>(i,j) = originalYpad.at<float>(by+i, bx+j) - 128.0f;
        Mat c; dct(block, c);
        Mat ac; absdiff(c, Scalar::all(0), ac);
        Mat logc; log(ac + 1e-6f, logc);
        normalize(logc, blockDCTVis, 0, 255, NORM_MINMAX);
        blockDCTVis.convertTo(blockDCTVis, CV_8U);
        // quantize that block to show quantized coefficients
        Mat qc = Mat::zeros(8,8,CV_32F);
        for (int i=0;i<8;i++) for (int j=0;j<8;j++) qc.at<float>(i,j) = round(c.at<float>(i,j) / Q.at<float>(i,j));
        Mat aq; absdiff(qc, Scalar::all(0), aq);
        Mat logq; log(aq + 1e-6f, logq);
        normalize(logq, blockQVis, 0, 255, NORM_MINMAX);
        blockQVis.convertTo(blockQVis, CV_8U);
        // original 8x8 luminance patch
        Mat orig8u = Mat::zeros(8,8,CV_8U);
        for (int i=0;i<8;i++) for (int j=0;j<8;j++) {
            float v = originalYpad.at<float>(by+i, bx+j);
            int iv = (int)round(v);
            orig8u.at<uchar>(i,j) = (uchar)iv;
        }
        blockOrig = orig8u;
    }

    // upscale small block visuals so they are visible
    Mat blockOrigLarge, blockDCTLarge, blockQLarge;
    resize(blockOrig, blockOrigLarge, Size(160,160), 0,0, INTER_NEAREST);
    resize(blockDCTVis, blockDCTLarge, Size(160,160), 0,0, INTER_NEAREST);
    resize(blockQVis, blockQLarge, Size(160,160), 0,0, INTER_NEAREST);

    // show windows
    namedWindow("Original", WINDOW_NORMAL); imshow("Original", img);
    namedWindow("Reconstructed (Y quantized)", WINDOW_NORMAL); imshow("Reconstructed (Y quantized)", reconBGR);
    namedWindow("Difference (abs)", WINDOW_NORMAL); imshow("Difference (abs)", diff);
    namedWindow("DCT log (Y channel)", WINDOW_NORMAL); imshow("DCT log (Y channel)", dctVis);
    namedWindow("Quantized coeffs log", WINDOW_NORMAL); imshow("Quantized coeffs log", qVis);
    namedWindow("8x8 block - orig / DCT / quant", WINDOW_NORMAL);
    Mat tile;
    hconcat(vector<Mat>{blockOrigLarge, blockDCTLarge, blockQLarge}, tile);
    imshow("8x8 block - orig / DCT / quant", tile);

    cout << "Quality used: " << quality << "\n";
    cout << "Press any key to exit windows.\n";

    waitKey(0);
    destroyAllWindows();
    return 0;
}