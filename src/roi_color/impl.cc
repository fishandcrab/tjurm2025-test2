#include "impls.h"
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <vector>
std::unordered_map<int, cv::Rect> roi_color(const cv::Mat& input) {
    /**
     * INPUT: 一张彩色图片, 路径: opencv/assets/roi_color/input.png
     * OUTPUT: 一个 unordered_map, key 为颜色(Blue: 0, Green: 1, Red: 2), value 为对应颜色的矩形区域(cv::Rect)
     * TODO:
     *  1. 找到图片中三个颜色的轮廓
     *  2. 计算出三处矩形的颜色
     *  3. 返回一个 unordered_map, key 为颜色, value 为cv::Rect (表示矩形的位置)
     * HINT:
     *  1. 预处理
     *    1. 在学习 findContours 时，我们提供的输入图片已经是一个二值图像了，
     *       但是在这里，input 是一个彩色图形。
     *       而 findContours 的 input 最好是一个二值图像。
     *    2. findContours 处理时，一般是在一个黑色背景上，找白色的轮廓
     *       (因此在二值化时，你可能需要考虑一下 cv::THRESH_BINARY_INV 选项
     *       同时，建议了解一下 cv::THRESH_OTSU 选项以及他们如何配合使用)
     *    3. 因此，预处理流程如下:
     *      1. 将 input 转换成灰度图像
     *      2. 对灰度图像进行二值化，得到黑底的二值化图
     *   2. 找到三个矩形 (findContours)
     *   3. 对找到的三个矩形，分别进行如下计算:
     *      1. 使用 cv::boundingRect 将轮廓转换成矩形 cv::Rect
     *      2. 使用该 cv::Rect 得到 input 中的 ROI区域 (写法: roi = input(rect))
     *      3. 使用统计的方法，得到该 ROI 区域的颜色
     *      4. 将颜色 和 矩形位置 存入 map 中
     */
    std::unordered_map<int, cv::Rect> res;
    // IMPLEMENT YOUR CODE HERE

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150);

    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> rects;
    for (size_t i = 0; i < contours.size(); i++) {
        if (hierarchy[i][3] == -1) { 
            rects.push_back(cv::boundingRect(contours[i]));
        }
    }

    if (rects.size() > 3) {
        rects.resize(3);
    }

    for (size_t i = 0; i < rects.size(); i++) {
        cv::Mat roi = input(rects[i]);
        cv::Scalar mean_color = cv::mean(roi);
        int color_index = 0;
        if (mean_color[2] > mean_color[1] && mean_color[2] > mean_color[0]) color_index = 2; // Red
        else if (mean_color[1] > mean_color[0] && mean_color[1] > mean_color[2]) color_index = 1; // Green
        else color_index = 0; // Blue
        res[color_index] = rects[i];
    }


    return res;
}
