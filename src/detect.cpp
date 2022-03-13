#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

#include "detect.h"
#include "roi.h"
#include "feature.h"
#include "func.h"

void detect::defect1()
{
    using namespace std;
    using namespace cv;

    /*
    * 读取样本
    */
    vector<string> REFER_LIST = { "refer000.BMP" };
    vector<string> DEFECT_LIST = { "defect000.BMP", "defect001.BMP" };
    string img_root_path = "../image/class1_defect1/";
    for (auto& i : REFER_LIST)
        i = img_root_path + i;

    for (auto& i : DEFECT_LIST)
        i = img_root_path + i;

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

        for (auto i : sample) {
            Mat image = imread(i, 0);
            image = image(Range(0, 800), Range(0, 800));
            
            /*
            * 阈值分割
            */
            int region_area = roi::thresholdSegment(image, AREA_PRECENT, PRE_AREA_NUM, structure_element);

            /*
            * 根据特征判断此样本是否合格，result为1表示合格，0表示有缺陷
            */
            int result = feature::regionArea(region_area, NORMAL_AREA);
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
    using namespace std;
    using namespace cv;

    /*
    * 读取样本
    */
    vector<string> REFER_LIST = { "refer000.BMP" };
    vector<string> DEFECT_LIST = { "defect000.BMP" };
    string img_root_path = "../image/class1_defect2/";
    for (auto& i : REFER_LIST)
        i = img_root_path + i;

    for (auto& i : DEFECT_LIST)
        i = img_root_path + i;

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

    string method = "template_match"; // 使用的分割方法
    string f = "ccoeff";  // 用来分类的特征

    if (method == "template_match") {
        int canny[2] = { 100, 200 };  // canny法的两个阈值

        /*
        * 生成模板
        */
        
        Mat target_template;
        target_template = func::templateGenerate(REFER_LIST, Range(20, 100),
                                                 Range(220, 470), "canny", canny);
        imshow("template", target_template);

        for (auto i : sample) {
            Mat image = imread(i, 0);
            float max_ccoeff = 0;
            Mat match_image;
            for (int i = 0; i <= 20; ++i) {
                float angle = i * 0.5 - 5;
                Mat M = getRotationMatrix2D(Point(image.cols / 2, image.rows / 2), angle, 1);
                Mat temp_image;
                warpAffine(image, temp_image, M, image.size());

                float CCOEFF = roi::templateMatch(temp_image, target_template, canny);
                
                if (CCOEFF >= max_ccoeff) {
                    max_ccoeff = CCOEFF;
                    match_image = temp_image;
                }
            }

            image = match_image;

            /*
            * 显示最终分离出的区域的图像
            */
            imshow("img" + to_string(count), image);

            /*
            * 根据特征判断此样本是否合格（合格为True）
            */
            if (f == "ccoeff") {
                cout << "sample " << count << "'s correlation coefficient is " << max_ccoeff << endl;
                int result = feature::correlation(max_ccoeff, 0.6, 0.2);
                results.push_back(result);
            }

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

void detect::templateMatch(const cv::Mat& image, const std::vector<cv::Mat>& templates, std::vector<int*> canny, const std::string& f, float* thresh) {
    using namespace std;
    using namespace cv;

    /*
    * 设定程序开始运行时间
    */
    clock_t time_start, time_end;
    time_start = clock();

    int t_count = 0;  // 模板计数
    for (const auto& template_img : templates) {
        int result = -1;  // 结果码
        if (f == "ccoeff") {
            Mat image_roi = image.clone();
            float CCOEFF = roi::templateMatch(image_roi, template_img, canny[t_count]);
            string message = "Region " + to_string(t_count + 1) + "'s correlation coefficient is " + to_string(CCOEFF);
            cout << message << endl;
            result = feature::correlation(CCOEFF, thresh[1], thresh[0]);
        }
        cout << result <<endl;
        func::resultExplain(result, t_count + 1);
        t_count ++;
    }

    time_end = clock();  // 记录程序结束运行时间
    cout << "detection running time: " << double(time_end - time_start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "---------------" << endl;
//    waitKey(0);
//    destroyAllWindows();
}
