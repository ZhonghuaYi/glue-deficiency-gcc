#include <iostream>
#include <opencv2/highgui.hpp>
#include <ctime>

#include "func.h"
#include "data.h"
#include "feature.h"
#include "roi.h"

void thresh_segment(cv::Mat image, float area_percent, const cv::Mat &structure_element, int normal_area,
                    float thresh) {
    using namespace std;
    using namespace cv;

    clock_t start_time, end_time;
    start_time = clock();

    image = image(Range(0, 800), Range(0, 800));
    int region_area = roi::thresholdSegment(image, area_percent, structure_element);

    // 显示最终分离出的区域的图像
//    imshow("image", image);

    // 根据特征判断此样本是否合格（合格为True）
    int result = feature::regionArea(region_area, normal_area, thresh);

    cout << "result is " << result << endl;

    end_time = clock();
    cout << "program running time: " << double(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

//    waitKey(0);
    destroyAllWindows();
}

int main() {
    using namespace std;
    using namespace cv;
    Data loader;
    loader.LoadData(1, "thresh_segment");
    int image_count = 1;
    for (auto &i: loader.sample) {
        cout << "-----------------------------" << endl;
        cout << "sample " << image_count << ":" << endl;
        Mat image = imread(i, 0);
        thresh_segment(image, loader.area_percent, loader.structure_element,
                       loader.normal_area, loader.thresh);
        image_count++;
    }
    return 0;
}
