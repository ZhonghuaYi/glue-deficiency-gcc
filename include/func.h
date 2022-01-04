#pragma once
#ifndef __FUNC_H_

#include<iostream>
#include<vector>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

struct SubRegion {
    std::vector<int> value;
    std::vector<int> area;
    std::vector<std::vector<int>> start;
};

namespace func {
    std::vector<int> linspace(float start, float stop, int num);
    std::vector<int> range(int num);
    void quickSortWithIndex(std::vector<int>& v, std::vector<int>& v_index, int start, int end);
    cv::Mat getHistogram(const cv::Mat& img);
    cv::Mat cdf(const cv::Mat& in_pic_histogram);
    int neighbor_expand(cv::Mat& img, int x, int y, int value, int region_area);
    SubRegion area_segment(cv::Mat& img, int pre_area_num);
}

#endif // !__FUNC_H_
