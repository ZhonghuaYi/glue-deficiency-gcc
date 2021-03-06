#pragma once
#ifndef _ROI_H_

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

namespace roi {
    int thresholdSegment(cv::Mat &image, float area_percent, const cv::Mat &structure_element);

    float templateMatch(cv::Mat &image, const cv::Mat &target_template, std::vector<int>canny, int flag=0);
}

#endif // !_ROI_H_

