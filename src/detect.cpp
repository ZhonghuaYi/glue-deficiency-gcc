#include<iostream>
#include<vector>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

#include "detect.h"
#include "segment.h"
#include "feature.h"
#include "func.h"

void detect::defect1()
{
    using namespace std;
    using namespace cv;

    /*
    * 读取样本
    */
    vector<string> REFER_LIST = { "test000.bmp" };
    vector<string> DEFECT_LIST = { "test001.bmp", "test002.bmp" };
    string img_root_path = "../image/";
    vector<string> sample;
    sample.insert(sample.end(), REFER_LIST.begin(), REFER_LIST.end());
    sample.insert(sample.end(), DEFECT_LIST.begin(), DEFECT_LIST.end());

    /*
    * 设定程序开始运行时间
    */
    clock_t time_start, time_end;
    time_start = clock();

    int count = 1;  // 图像的计数
    vector<int> results; // 存储样本的判断结果

    string method = "thresh"; // 使用的分割方法
    string f = "ccoeff";  // 用来分类的特征

    if (method == "thresh") {
        float AREA_PRECENT = 0.3;  // 灰度阈值的面积占比
        int PRE_AREA_NUM = 12;  // 预设的最大区域数量
        int NORMAL_AREA = 420;  // 目标区域的完整面积

        /*
        * 用于形态学计算的矩形结构元素
        */
        Mat structure_element = getStructuringElement(MORPH_RECT, Size(7, 7));
        vector<int> target_region_areas;

        vector<string>::iterator i;
        for (string i : sample) {
            Mat image = imread(img_root_path + i, 0);
            
            /*
            * 阈值分割
            */
            int region_area = segment::threshold_segment(image, AREA_PRECENT, PRE_AREA_NUM, structure_element);

            /*
            * 根据特征判断此样本是否合格，result为1表示合格，0表示有缺陷
            */
            int result = feature::region_area(region_area, NORMAL_AREA);
            results.push_back(result);

            /*
            * 显示最终分离出的区域的图像
            */
            namedWindow("img" + to_string(count));
            imshow("img" + to_string(count), image);
            count++;
        }

        cout << "results: " << endl;
        for (auto i : results) {
            if (i == 1)
                cout << "True ";
            else
                cout << "Flase ";
        }

        time_end = clock();
        cout << endl << "program running time: " << double(time_end - time_start) / CLOCKS_PER_SEC << "s";

        waitKey(0);
        destroyAllWindows();
    }

    
}

void detect::defect2() {

}
