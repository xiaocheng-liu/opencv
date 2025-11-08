#include <opencv2/core.hpp>
#include <iostream>
#include <vector>

#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/photo.hpp"
using namespace cv;
using namespace std;

// ========== PARAMETERS (tune these) ==========
const double TEMPLATE_THRESH = 0.65; // template match threshold (0..1)
const bool DO_MULTISCALE_TEMPLATE = true; // try multiple template scales
const int TEMPLATE_MIN_SIDE = 20; // smallest template side (skip too tiny)
const int DILATE_K = 9;   // dilate kernel size for mask (covers semi-transparent edges)
const int INPAINT_RADIUS = 3; // radius for inpaint
const int MIN_REGION_AREA = 50; // filtration for small blobs
// ==============================================

// simple non-max suppression on match map: mark detection and clear neighborhood
void markTemplateMatches(const Mat &matchMap, Mat &mask, Size tplSize, double thresh) {
    Mat norm;
    normalize(matchMap, norm, 0, 1, NORM_MINMAX, CV_32F);
    Mat visited = Mat::zeros(norm.size(), CV_8U);

    for (int y = 0; y < norm.rows; ++y) {
        for (int x = 0; x < norm.cols; ++x) {
            if (visited.at<uchar>(y, x)) continue;
            float v = norm.at<float>(y, x);
            if (v >= thresh) {
                Rect r(x, y, tplSize.width, tplSize.height);
                r &= Rect(0,0,mask.cols, mask.rows);
                mask(r) = 255;
                // suppress neighborhood
                int nx = max(0, x - tplSize.width/2);
                int ny = max(0, y - tplSize.height/2);
                int wx = min(norm.cols - nx, tplSize.width);
                int wy = min(norm.rows - ny, tplSize.height);
                if (wx>0 && wy>0) norm(Range(ny, ny+wy), Range(nx, nx+wx)).setTo(0);
                // mark visited area roughly
                if (r.height>0 && r.width>0) visited(r) = 1;
            }
        }
    }
}

// multi-scale template matching (fast naive approach)
void templateMultiScale(const Mat &imgGray, const Mat &tplGray, Mat &mask, double thresh) {
    if (tplGray.cols < TEMPLATE_MIN_SIDE || tplGray.rows < TEMPLATE_MIN_SIDE) return;
    vector<double> scales = {1.0, 0.9, 0.8, 1.1, 1.2}; // try scales
    for (double s : scales) {
        Mat tpl;
        resize(tplGray, tpl, Size(), s, s, INTER_AREA);
        if (tpl.cols < 8 || tpl.rows < 8) continue;
        Mat result;
        int rw = imgGray.cols - tpl.cols + 1;
        int rh = imgGray.rows - tpl.rows + 1;
        if (rw <= 0 || rh <= 0) continue;
        result.create(rh, rw, CV_32FC1);
        matchTemplate(imgGray, tpl, result, TM_CCOEFF_NORMED);
        markTemplateMatches(result, mask, tpl.size(), thresh);
    }
}

// MSER-based candidate detection (good for stamped logos / text blobs)
Mat detectMSERCandidates(const Mat &imgGray) {
    Ptr<MSER> mser = MSER::create(21, 60, 14400, 0.25, 0.2, 200, 1.01, 0.003, 5);
    vector<vector<Point>> regions;
    vector<Rect> boxes;
    mser->detectRegions(imgGray, regions, boxes);
    Mat mask = Mat::zeros(imgGray.size(), CV_8U);
    for (auto &r : regions) {
        Rect b = boundingRect(r);
        if (b.area() < 30) continue;
        // heuristic: thin or elongated regions may be text/watermark
        drawContours(mask, vector<vector<Point>>{r}, -1, Scalar(255), FILLED);
    }
    return mask;
}

// edge / contour based candidates
Mat detectEdgeCandidates(const Mat &imgGray) {
    Mat blurred;
    GaussianBlur(imgGray, blurred, Size(5,5), 0);
    Mat edges;
    Canny(blurred, edges, 50, 150);

    // 确保 edges 是二值图像
    Mat binaryEdges;
    threshold(edges, binaryEdges, 0, 255, THRESH_BINARY);

    // 膨胀操作
    Mat kern = getStructuringElement(MORPH_RECT, Size(3,3));
    dilate(binaryEdges, binaryEdges, kern, Point(-1,-1), 2);

    // 检测轮廓
    vector<vector<Point>> contours;
    findContours(binaryEdges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 转换为填充区域
    Mat mask = Mat::zeros(binaryEdges.size(), CV_8U);
    for (const auto& contour : contours) {
        drawContours(mask, vector<vector<Point>>{contour}, -1, Scalar(255), FILLED);
    }

    // 闭操作填充孔洞
    morphologyEx(mask, mask, MORPH_CLOSE, kern, Point(-1,-1), 2);
    return mask;
}


// refine mask: open, close, dilate and keep only significant blobs
Mat refineMask(const Mat &raw) {
    Mat m = raw.clone();
    if (m.channels() != 1) cvtColor(m, m, COLOR_BGR2GRAY);
    threshold(m, m, 10, 255, THRESH_BINARY);
    Mat k1 = getStructuringElement(MORPH_ELLIPSE, Size(3,3));
    morphologyEx(m, m, MORPH_OPEN, k1, Point(-1,-1), 1);
    Mat k2 = getStructuringElement(MORPH_ELLIPSE, Size(DILATE_K, DILATE_K));
    dilate(m, m, k2);
    // keep only reasonably sized contours
    vector<vector<Point>> cnts;
    findContours(m.clone(), cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Mat finalMask = Mat::zeros(m.size(), CV_8U);
    for (auto &c : cnts) {
        double area = contourArea(c);
        if (area >= MIN_REGION_AREA) {
            drawContours(finalMask, vector<vector<Point>>{c}, -1, Scalar(255), FILLED);
        }
    }
    // feather edges
    Mat feather;
    GaussianBlur(finalMask, feather, Size(9,9), 0);
    threshold(feather, finalMask, 10, 255, THRESH_BINARY);
    return finalMask;
}

// main per-image pipeline
bool processImage(const string &imgPath, const vector<string> &tplPaths) {
    Mat img = imread(imgPath, IMREAD_COLOR);
    if (img.empty()) { cerr<<"Fail to open "<<imgPath<<"\n"; return false; }
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    Mat rawMask = Mat::zeros(img.size(), CV_8U);

    // 1) template matching if templates provided
    for (auto &tp : tplPaths) {
        Mat tpl = imread(tp, IMREAD_GRAYSCALE);
        if (tpl.empty()) { cerr<<"Warn: cannot open template "<<tp<<"\n"; continue; }
        templateMultiScale(gray, tpl, rawMask, TEMPLATE_THRESH);
    }

    // 2) if rawMask empty, fallback auto detection (MSER + edges)
    if (countNonZero(rawMask) < 10) {
        Mat m1 = detectMSERCandidates(gray);
        Mat m2 = detectEdgeCandidates(gray);
        rawMask = rawMask | m1 | m2;
    }

    // 3) refine mask
    Mat mask = refineMask(rawMask);

    // 4) small-area improvement: if mask large, consider splitting into tiles (not implemented here)
    Mat inpainted;
    if (countNonZero(mask) == 0) {
        cerr<<"No mask detected for "<<imgPath<<", skipping inpaint.\n";
        return false;
    } else {
        inpaint(img, mask, inpainted, INPAINT_RADIUS, INPAINT_TELEA);
    }

    // 5) save outputs
    string outBase = imgPath;
    size_t pos = outBase.find_last_of("/\\");
    string name = (pos==string::npos)? outBase : outBase.substr(pos+1);
    imwrite("mask_"+name, mask);
    imwrite("inpainted_"+name, inpainted);

    imshow("orig", img);
    imshow("mask", mask);
    imshow("inpainted", inpainted);
    waitKey(1); // short show - for batch change to 0 to pause
    cout<<"Processed: "<<imgPath<<"  -> saved mask_"<<name<<" inpainted_"<<name<<"\n";
    return true;
}

int main(int argc, char** argv) {
    string imgPath = samples::findFile("Water.jpg");
    vector<string> tplPaths;
    for (int i=2;i<argc;i++) tplPaths.push_back(string(argv[i]));
    processImage(imgPath, tplPaths);
    // For batch mode you can call processImage in a loop over files
    return 0;
}