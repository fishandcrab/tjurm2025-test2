#include "impls.h"
  #include <opencv2/opencv.hpp>

cv::Mat my_resize(const cv::Mat& input, float scale) {
    /**
     * 要求：
     *      实现resize算法，只能使用基础的语法，比如说for循环，Mat的基本操作。不能
     * 用cv::resize。resize算法的内容自行查找学习，不是很难。
     * 
     * 提示：
     * 无。
     * 
     * 通过条件：
     * 运行测试点，你的结果跟答案长的差不多就行。
     */
    int new_rows = input.rows * scale, new_cols = input.cols * scale;
    // IMPLEMENT YOUR CODE HERE


    cv::Mat output = cv::Mat::zeros(new_rows, new_cols, input.type());

    for (int i = 0; i < new_rows; ++i) {
        for (int j = 0; j < new_cols; ++j) {
         
            float x = j / scale;
            float y = i / scale;

            int x0 = static_cast<int>(std::floor(x));
            int y0 = static_cast<int>(std::floor(y));
            int x1 = (x0 < input.cols - 1) ? x0 + 1 : x0;
            int y1 = (y0 < input.rows - 1) ? y0 + 1 : y0;

            float dx = x - x0;
            float dy = y - y0;

            cv::Vec3b color_top_left = input.at<cv::Vec3b>(y0, x0);
            cv::Vec3b color_top_right = input.at<cv::Vec3b>(y0, x1);
            cv::Vec3b color_bottom_left = input.at<cv::Vec3b>(y1, x0);
            cv::Vec3b color_bottom_right = input.at<cv::Vec3b>(y1, x1);

            cv::Vec3b interpolated_color;
            interpolated_color[0] = (1 - dx) * (1 - dy) * color_top_left[0] + dx * (1 - dy) * color_top_right[0] +
                                   (1 - dx) * dy * color_bottom_left[0] + dx * dy * color_bottom_right[0];
            interpolated_color[1] = (1 - dx) * (1 - dy) * color_top_left[1] + dx * (1 - dy) * color_top_right[1] +
                                   (1 - dx) * dy * color_bottom_left[1] + dx * dy * color_bottom_right[1];
            interpolated_color[2] = (1 - dx) * (1 - dy) * color_top_left[2] + dx * (1 - dy) * color_top_right[2] +
                                   (1 - dx) * dy * color_bottom_left[2] + dx * dy * color_bottom_right[2];

            output.at<cv::Vec3b>(i, j) = interpolated_color;
        }
    }

    return output;

  //  return cv::Mat::zeros(new_rows, new_cols, input.type());
}