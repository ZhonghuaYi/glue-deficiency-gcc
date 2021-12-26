#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<string> REFER_LIST = { "test000.bmp" };
vector<string> DEFECT_LIST = { "test001.bmp", "test002.bmp" };
int PRE_AREA_NUM = 12;

int main() {
    vector<string> sample;
    sample.insert(sample.end(), REFER_LIST.begin(), REFER_LIST.end());
    sample.insert(sample.end(), DEFECT_LIST.begin(), DEFECT_LIST.end());

    int count = 1;
    Mat structure_element = getStructuringElement(MORPH_RECT, Size(7, 7));
    vector<int> target_region_areas;

    vector<string>::iterator i;
    for (i = sample.begin(); i != sample.end(); ++i) {
        Mat image = imread(*i, 1);
        Mat hist = calcHist()
    }

    for (vector<int>::size_type i = 0; i < target_region_areas.size(); ++i)
        cout << target_region_areas[i] << endl;
    waitKey(0);
    return 0;
}
