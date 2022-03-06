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
    string f = "";
    float thresh[2] = { 0, 0 };
    if (sample_set == 1) {
        /*
        * 读取样本
        */
        string sample_root = "../image/sample/";
    }
    return 0;
}
