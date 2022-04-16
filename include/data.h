#ifndef GLUE_DEFICIENCY_GCC_DATA_H
#define GLUE_DEFICIENCY_GCC_DATA_H
#include <vector>
#include <string>
#include <opencv2/core.hpp>

namespace data{
    extern std::vector<std::string> sample;
    extern float area_percent, thresh;
    extern int normal_area;
    extern cv::Mat structure_element;
    extern std::vector<cv::Mat> edge_templates, templates;
    extern std::vector<int*> canny;
    void LoadData(const int& sample_set, const std::string& segment);
}

#endif //GLUE_DEFICIENCY_GCC_DATA_H
