#pragma once
#ifndef __FUNC_H_

#include<filesystem>
#include<iostream>
#include<vector>
#include<opencv2/core.hpp>

struct SubRegion {
    std::vector<int> value;
    std::vector<int> area;
    std::vector<std::vector<int>> start;
};

namespace func {
    cv::Mat imageResize(const cv::Mat &image, int size);

    std::vector<int> linspace(float start, float stop, int num);

    std::vector<int> range(int num);

    void quickSortWithIndex(std::vector<int> &v, std::vector<int> &v_index, int start, int end);

    cv::Mat getHistogram(const cv::Mat &img);

    cv::Mat cdf(const cv::Mat &in_pic_histogram);

    std::vector<std::string> referGenerate(const std::string &dir_path);

    std::vector<std::string> sampleGenerate(const std::string &dir_path,
                                            const std::vector<std::string> &sample_list = std::vector<std::string>());

    int neighborExpand(cv::Mat &img, int x, int y, int value, int region_area);

    SubRegion areaSegment(cv::Mat &img);

    std::vector<cv::Mat>
    templateGenerate(const std::vector<std::string> &refer_sample, cv::Range row_wise, cv::Range col_wise,
                     const std::string &flag = "canny", std::vector<int> canny = std::vector<int>(), int thresh = 50);

    std::vector<cv::Mat> gaussianPyramid(cv::Mat image, const std::string &flag, int num);

    void nearestPoint(cv::KeyPoint& point, std::vector<cv::KeyPoint>& pt_set, double& min_distance, int& index);

    std::vector<cv::DMatch> pointLocationMatch(std::vector<cv::KeyPoint>& kp_t, std::vector<cv::KeyPoint>& kp_img,
                                               double th);

    void drawLine(cv::Mat &drawing, std::vector<cv::Vec4i> &lines);

    std::vector<cv::Vec4i> defect1HoughLine(const cv::Mat &image);

    std::vector<cv::Vec4i> defect2HoughLine(const cv::Mat &image);

    void resultExplain(int result, int n);
}

#endif // !__FUNC_H_
