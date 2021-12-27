#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<string> REFER_LIST = { "test000.bmp" };
vector<string> DEFECT_LIST = { "test001.bmp", "test002.bmp" };
int PRE_AREA_NUM = 12;

/// <summary>
/// get normalized histogram.
/// </summary>
/// <param name="img"></param>
/// <returns>
/// normalized histogram of input image.
/// </returns>
Mat getHistogram(const Mat& img) {
    Mat hist = Mat::zeros(Size(256, 1), CV_32FC1);
    Mat out;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int index = int(img.at<uchar>(i, j));
            hist.at<float>(0, index) += 1;
        }
    }
    out = hist / img.total();
    return out;
}

/// <summary>
/// get the cumulative distribution fuction(CDF) of image.
/// </summary>
/// <param name="in_pic_histogram"></param>
/// <returns>
/// CDF of the image.
/// </returns>
Mat cdf(const Mat& in_pic_histogram) {
    int hist_size = in_pic_histogram.total();
    Mat transform = Mat::zeros(in_pic_histogram.size(), in_pic_histogram.depth());
    float temp = 0.;
    for (int i = 0; i < hist_size; ++i) {
        temp += in_pic_histogram.at<float>(0, i);
        transform.at<float>(0, i) = temp;
    }
    transform = transform * (hist_size - 1);
    transform.convertTo(transform, CV_32SC1);
    transform.convertTo(transform, in_pic_histogram.depth());
    return transform;
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
    for (string i :sample) {
        Mat image = imread(img_root_path + i, 0);
        Mat hist;
        
        /*
        * opencv计算直方图的官方方法
        int channels[] = { 0 };
        const int histSize[] = { 256 };
        float range[] = { 0, 256 };
        const float* ranges[] = { range };
        calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
        */


        /*
        * 在参考图像中，当手动阈值在37时，阈值分割效果明显。于是考虑到灰度小于37的区域大概面积占比是0.3，
        * 于是将图像中灰度值较低的30%区域分割出来。这里利用了cdf，它本身是直方图的累积分布，因此只需要寻找
        * cdf中最接近0.3的位置，其索引即是能够将30%灰度比较低的区域分割出来的阈值
        */
        hist = getHistogram(image);
        //cout << hist << endl;
        Mat img_cdf = cdf(hist) / 255.;
        int point[2] = {};
        minMaxIdx(abs(img_cdf - 0.3), 0, 0, point, 0);
        int index = point[1];


        /*
        * 得到阈值后，对图像进行阈值分割，然后对不规则区域应用中值滤波平滑。
        * 每次平滑后，需要通过膨胀背景从而腐蚀物体，以使得目标区域能够更容易被分离
        */
        medianBlur(image, image, 9);
        dilate(image, image, structure_element);

        resize(image, image, Size(400, 400));
        medianBlur(image, image, 3);
        dilate(image, image, structure_element);

        resize(image, image, Size(70, 70));
        threshold(image, image, index, 255, THRESH_BINARY);
        Mat compare = Mat::ones(image.size(), image.depth()) * 255;
        Mat diff = compare == image;
        cout << diff << endl;

        namedWindow("img");
        imshow("img", diff);
        break;
    }

    for (int target_region_area : target_region_areas)
        cout << target_region_area << endl;
    waitKey(0);
    destroyAllWindows();
    return 0;
}
