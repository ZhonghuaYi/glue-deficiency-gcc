#pragma once
#ifndef _FEATURE_H_

#include <opencv2/core.hpp>
#include <vector>

namespace feature {
    int regionArea(int area, int normal_area, float thresh);

    int correlation(float ccoeff, float th, float th0);

    int defect1Hough(float img_rows, float img_cols, std::vector<cv::Vec4i> lines, int max_lines);

    int defect2Hough(float img_rows, float img_cols, std::vector<cv::Vec4i> lines, int max_lines);
}

#endif // !_FEATURE_H_
