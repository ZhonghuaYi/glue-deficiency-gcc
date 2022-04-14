#include <iostream>
#include <opencv2/highgui.hpp>
#include <ctime>

#include "func.h"
#include "data.h"
#include "feature.h"
#include "roi.h"

void thresh_segment(cv::Mat image, float area_percent, cv::Mat structure_element, int normal_area, float thresh){
    using namespace std;
    using namespace cv;

    clock_t start_time, end_time;
    start_time = clock();



    end_time = clock();
    cout << endl << "program running time: " << double(end_time - start_time) / CLOCKS_PER_SEC << "s";

    waitKey(0);
    destroyAllWindows();
}

int main() {

    return 0;
}
