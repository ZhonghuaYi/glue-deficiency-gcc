#include <iostream>
#include "func.h"
#include "detect.h"


int main() {
    using namespace std;
    using namespace cv;
    using namespace detect;
    
    int sample_set = 1;
    vector<string> sample;
    vector<Mat> templates;
    vector<int[2]>canny;
    string f;
    float thresh[2];
    if (sample_set == 1) {
        /*
        * 读取样本
        */
        string sample_root = "../image/sample/";
        sample = func::sample_generate(sample_root);
        // defect1();
        // defect2();

        /*
         * 读取参考样本
         */
        string refer1_root = "../image/refer1/";
        vector<string> refer1_sample = func::refer_generate(refer1_root);
        string refer2_root = "../image/refer2/";
        vector<string> refer2_sample = func::refer_generate(refer2_root);

        int canny1[2] = {50, 100};
        int canny2[2] = {100, 200};

        f = "ccoeff";
        thresh[0] = 0.2;
        thresh[1] = 0.6;

        /*
         * 生成模板
         */

    }
    else if (sample_set == 2){
        string sample_root = "../image2/sample/";
    }
    return 0;
}
