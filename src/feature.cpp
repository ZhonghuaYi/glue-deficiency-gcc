#include "feature.h"
#include <iostream>

/// <summary>
/// 用检测到的区域与没有缺陷时的区域面积比值判断图像是否有缺陷
/// </summary>
/// <param name="area">: 检测到的区域面积 </param>
/// <param name="normal_area">: 没有缺陷时的区域的面积 </param>
/// <returns></returns>
int feature::regionArea(const int area, const int normal_area, float thresh) {
    if (float(area) / float(normal_area) < thresh)
        return 0;
    else
        return 1;
}

/// <summary>
/// 用相关系数判断图像是否有缺陷
/// </summary>
/// <param name="ccoeff">: 相关系数 </param>
/// <returns></returns>
int feature::correlation(const float ccoeff, float th, float th0) {
    if (ccoeff < th0)
        return 2;
    else if (th0 <= ccoeff && ccoeff < th)
        return 0;
    else
        return 1;
}

int feature::defect1Hough(float img_rows, float img_cols, std::vector<cv::Vec4i> lines, int max_lines) {
    int line_count = 0;
    // 设定关注的区域范围
    float feature_zone[4] = {img_cols / 3, img_cols * 2 / 3, img_rows / 4, img_rows * 3 / 4};
    // 当直线位于关注的区域范围内时，累计一条直线
    for (auto &line: lines) {
        auto x1 = float(line[0]), y1 = float(line[1]), x2 = float(line[2]), y2 = float(line[3]);
        if (x1 > feature_zone[0] && x1 < feature_zone[1] &&
            y1 > feature_zone[2] && y1 < feature_zone[3] &&
            x2 > feature_zone[0] && x2 < feature_zone[1] &&
            y2 > feature_zone[2] && y2 < feature_zone[3])
            line_count += 1;
    }
    std::cout << "Lines' count in target region: " << line_count << std::endl;
    if (line_count <= max_lines)
        return 1;
    else
        return 0;
}

int feature::defect2Hough(float img_rows, float img_cols, std::vector<cv::Vec4i> lines, int max_lines) {
    int line_count = 0;
    // 设定关注的区域范围
    float feature_zone[4] = {0, img_cols / 2, img_rows / 2, img_rows};
    // 当直线位于关注的区域范围内时，累计一条直线
    for (auto &line: lines) {
        auto x1 = float(line[0]), y1 = float(line[1]), x2 = float(line[2]), y2 = float(line[3]);
        if (x1 > feature_zone[0] && x1 < feature_zone[1] &&
            y1 > feature_zone[2] && y1 < feature_zone[3] &&
            x2 > feature_zone[0] && x2 < feature_zone[1] &&
            y2 > feature_zone[2] && y2 < feature_zone[3])
            line_count += 1;
    }
    std::cout << "Lines' count in target region: " << line_count << std::endl;
    if (line_count <= max_lines)
        return 1;
    else
        return 0;
}

int
feature::keyPoints(std::vector<cv::KeyPoint> &kp_t, std::vector<cv::KeyPoint> &kp_img, std::vector<cv::DMatch> &matches,
                   double th, double th0, double min_distance) {
    double match_percent;
    match_percent = double(matches.size()) / (double(kp_t.size())+double(kp_img.size())-double(matches.size()));
    std::cout << "match percent: "<<match_percent<<std::endl;
    if(match_percent < th0)
        return 2;
    else if(match_percent >= th0 && match_percent < th)
        return 0;
    else{
        double distance_sum = 0, average_distance;
        for(auto &m : matches)
            distance_sum += m.distance;
        average_distance = distance_sum / double(matches.size());
        if(average_distance < min_distance)
            return 1;
        else
            return 0;
    }
}
