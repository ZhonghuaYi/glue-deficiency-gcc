#ifndef GLUE_DEFICIENCY_GCC_DATA_H
#define GLUE_DEFICIENCY_GCC_DATA_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>

class Data {
public:
    std::vector<std::string> sample;
    float area_percent, thresh;
    int normal_area;
    std::vector<float> t_thresh;
    std::string f;
    cv::Mat structure_element;
    std::vector<cv::Mat> edge_templates, templates;
    std::vector<std::vector<int>> canny;

    void LoadData(const int &sample_set, const std::string &segment);
};

#endif //GLUE_DEFICIENCY_GCC_DATA_H
