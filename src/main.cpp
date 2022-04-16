#include <iostream>
#include <opencv2/highgui.hpp>
#include <ctime>

#include "func.h"
#include "data.h"
#include "feature.h"
#include "roi.h"

void thresh_segment(cv::Mat image, float area_percent, const cv::Mat& structure_element, int normal_area, float thresh){
    using namespace std;
    using namespace cv;

    clock_t start_time, end_time;
    start_time = clock();

    image = image(Range(0, 800), Range(0, 800));
    int region_area = roi::thresholdSegment(image, area_percent, structure_element);

    // 显示最终分离出的区域的图像
    imshow("image", image);

    // 根据特征判断此样本是否合格（合格为True）
    int result = feature::regionArea(region_area, normal_area, thresh);

    cout << "result is " << result << endl;

    end_time = clock();
    cout << endl << "program running time: " << double(end_time - start_time) / CLOCKS_PER_SEC << "s";

    waitKey(0);
    destroyAllWindows();
}

int main() {
    data::LoadData(1, "thresh_segment");
    for (auto& i:data::sample){
        cv::Mat image = cv::imread(i, 0);
        thresh_segment(image, data::area_percent, data::structure_element, data::normal_area, data::thresh);
    }
    return 0;
}
