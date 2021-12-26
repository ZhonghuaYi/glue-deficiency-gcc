#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<string> REFER_LIST = { "test000.bmp" };
vector<string> DEFECT_LIST = { "test001.bmp", "test002.bmp" };
int PRE_AREA_NUM = 12;

Mat getHistogram(const Mat& img) {
    Mat hist = Mat::zeros(Size(256, 1), CV_32FC1);
    Mat img_size, out;
    multiply(Mat::ones(hist.size(), hist.depth()), img.total(), img_size, 1, CV_32FC1);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int index = int(img.at<uchar>(i, j));
            hist.at<float>(0, index) += 1;
        }
    }
    divide(hist, img_size, out);
    return out;
}

int main() {
    string img_root_path = "../image/";
    vector<string> sample;
    sample.insert(sample.end(), REFER_LIST.begin(), REFER_LIST.end());
    sample.insert(sample.end(), DEFECT_LIST.begin(), DEFECT_LIST.end());

    int count = 1;
    Mat structure_element = getStructuringElement(MORPH_RECT, Size(7, 7));
    vector<int> target_region_areas;

    vector<string>::iterator i;
    for (i = sample.begin(); i != sample.end(); ++i) {
        Mat image = imread(img_root_path + *i, 1);
        Mat hist;
        vector<float> range = { 0, 256 };
        //calcHist(&image, 1, 0, Mat(), hist, 1, 256, range);
        hist = getHistogram(image);
            
        break;
    }

    for (int target_region_area : target_region_areas)
        cout << target_region_area << endl;
    waitKey(0);
    return 0;
}
