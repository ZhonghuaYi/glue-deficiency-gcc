#include "data.h"
#include "func.h"

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void data::LoadData(int sample_set) {
    using namespace std;
    using namespace cv;
    using namespace func;
    if (sample_set == 1){
        vector<Mat> edge_templates;
        vector<Mat> templates;

        string segment = "template_match";
        // 样本图片的位置
        string sample_root = "./image/sample/";
        string refer1_root = "./image/refer1/";
        string refer2_root = "./image/refer2/";

        // 读取样本
        vector<string> sample = func::sampleGenerate(sample_root);

        // 读取参考样本
        vector<string> refer1_sample = func::referGenerate(refer1_root);
        vector<string> refer2_sample = func::referGenerate(refer2_root);

        if (segment == "thresh_segment"){
            float area_percent = 0.3;
            int normal_area = 420;
            float thresh = 0.9;

            // 用于形态学计算的矩形结构元素
            Mat structure_element = cv::getStructuringElement(MORPH_RECT, Size(7, 7));
        }
        else if(segment == "template_match"){
            int canny[2] = {50, 100};
        }
    }
}
