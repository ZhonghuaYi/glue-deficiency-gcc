#include <iostream>
#include "func.h"
#include "detect.h"
#include <opencv2/highgui.hpp>


int main() {
    using namespace std;
    using namespace cv;
    using namespace detect;
    
    int sample_set = 1;
    vector<string> sample;
    vector<Mat> templates;
    vector<int[2]>canny_set;
    string f;
    float thresh[2];
    int imread_flag = 0;
    if (sample_set == 1) {
        /*
        * 读取样本
        */
        string sample_root = "../image/sample/";
        sample = func::sampleGenerate(sample_root);
        // defect1();
        // defect2();

        /*
         * 读取参考样本
         */
        string refer1_root = "../image/refer1/";
        vector<string> refer1_sample = func::referGenerate(refer1_root);
        string refer2_root = "../image/refer2/";
        vector<string> refer2_sample = func::referGenerate(refer2_root);

        int canny1[2] = {50, 100};
        int canny2[2] = {100, 200};

        f = "ccoeff";
        thresh[0] = 0.2;
        thresh[1] = 0.6;

        /*
         * 生成模板
         */
        Mat template1 = func::templateGenerate(refer1_sample, Range(50, 300),
                                               Range(50, 300), "canny", canny1);
        templates.push_back(template1);
        Mat template2 = func::templateGenerate(refer2_sample, Range(20, 100),
                                               Range(220, 470), "canny", canny2);
        templates.push_back(template2);
        for (int i=0;i<templates.size();i++){
            string window_name = "template" + to_string(i);
            imshow(window_name, templates[i]);
        }
        waitKey(0);
        destroyAllWindows();
    }
    else if (sample_set == 2){
        /*
        * 读取样本
        */
        string sample_root = "../image2/sample/";
        sample = func::sampleGenerate(sample_root);

        /*
         * 读取参考样本
         */
        string refer_root = "../image2/refer/";
        vector<string> refer_sample = func::referGenerate(refer_root);

        int canny1[2] = {0, 200};
        for(int i=0;i<5;i++)
            canny_set.push_back(canny1);

        f = "ccoeff";
        thresh[0] = 0.1;
        thresh[1] = 0.4;

        /*
         * 生成模板
         */
        Mat template1 = func::templateGenerate(refer_sample, Range(150, 300),
                                               Range(120, 270), "canny", canny1);
        templates.push_back(template1);
        Mat template2 = func::templateGenerate(refer_sample, Range(170, 270),
                                               Range(230, 330), "canny", canny1);
        templates.push_back(template2);
        Mat template3 = func::templateGenerate(refer_sample, Range(70, 150),
                                               Range(300, 400), "canny", canny1);
        templates.push_back(template3);
        Mat template4 = func::templateGenerate(refer_sample, Range(250, 350),
                                               Range(250, 350), "canny", canny1);
        templates.push_back(template4);
        Mat template5 = func::templateGenerate(refer_sample, Range(300, 400),
                                               Range(100, 250), "canny", canny1);
        templates.push_back(template5);
        for (int i=0;i<templates.size();i++){
            string window_name = "template" + to_string(i);
            imshow(window_name, templates[i]);
        }
        waitKey(0);
        destroyAllWindows();
    }

    int count = 1;
    for(const auto& s : sample){
        Mat image = imread(s, imread_flag);
        templateMatch(image, templates, canny_set, f, thresh);
        count ++;
    }

    return 0;
}
