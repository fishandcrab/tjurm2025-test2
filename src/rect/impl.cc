#include "impls.h"
#include <opencv2/opencv.hpp>

std::pair<cv::Rect, cv::RotatedRect> get_rect_by_contours(const cv::Mat& input) {
    /**
     * 要求：
     * 使用findCountours函数找出输入图像（彩色图像）中的矩形轮廓，并且返回它的外接矩形，以及
     * 最小面积外接矩形。所需要的函数自行查找。
     * 
     * 通过条件：
     * 运行测试点，你找到的矩形跟答案一样就行。
    */
    std::pair<cv::Rect, cv::RotatedRect> res;
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

    cv::Rect boundingRect;
    cv::RotatedRect minAreaRect;
    bool found = false;

    for (size_t i = 0; i < contours.size(); i++) {
      
        boundingRect = cv::boundingRect(contours[i]);
        minAreaRect = cv::minAreaRect(contours[i]);

        if (cv::isContourConvex(contours[i]) && (minAreaRect.size.width > 0) && (minAreaRect.size.height > 0)) {
            found = true;
            break; 
        }
    }

    if (!found) {
       
        res.first = cv::Rect();
        res.second = cv::RotatedRect();
    } else {
      
        res.first = boundingRect;
        res.second = minAreaRect;
    }

   
    return res;
}