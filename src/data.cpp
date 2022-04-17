#include "data.h"
#include "func.h"

#include <opencv2/imgproc.hpp>

void Data::LoadData(const int &sample_set, const std::string &segment) {
    using namespace std;
    using namespace cv;
    using namespace func;
    if (sample_set == 1) {
        // 样本图片的位置
        string sample_root = "../image/sample/";
        string refer1_root = "../image/refer1/";
        string refer2_root = "../image/refer2/";

        // 读取样本
        sample = func::sampleGenerate(sample_root);

        // 读取参考样本
        vector<string> refer1_sample = func::referGenerate(refer1_root);
        vector<string> refer2_sample = func::referGenerate(refer2_root);

        if (segment == "thresh_segment") {
            area_percent = 0.3;
            normal_area = 420;
            thresh = 0.9;

            // 用于形态学计算的矩形结构元素
            structure_element = cv::getStructuringElement(MORPH_RECT, Size(7, 7));
        } else if (segment == "template_match") {
            int canny1[2] = {50, 100};
            int canny2[2] = {10, 200};
            canny.push_back(canny1);
            canny.push_back(canny2);
            string f;
            f = "sift";

            // 生成模板
            vector<Mat> t_out;
            t_out = func::templateGenerate(refer1_sample, Range(50, 300), Range(50, 300), "canny", canny1);
            templates.push_back(t_out[0]);
            edge_templates.push_back(t_out[1]);
            t_out = func::templateGenerate(refer2_sample, Range(20, 100), Range(220, 470), "canny", canny2);
            templates.push_back(t_out[0]);
            edge_templates.push_back(t_out[1]);
        }
    }
}
