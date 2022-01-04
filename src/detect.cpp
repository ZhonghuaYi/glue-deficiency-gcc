#include<iostream>
#include<vector>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include "detect.h"
#include "func.h"

void detect::class1Defect1()
{
    using namespace std;
    using namespace cv;

    vector<string> REFER_LIST = { "test000.bmp" };
    vector<string> DEFECT_LIST = { "test001.bmp", "test002.bmp" };
    int PRE_AREA_NUM = 12;

    clock_t time_start, time_end;
    time_start = clock();

    string img_root_path = "../image/";
    vector<string> sample;
    sample.insert(sample.end(), REFER_LIST.begin(), REFER_LIST.end());
    sample.insert(sample.end(), DEFECT_LIST.begin(), DEFECT_LIST.end());

    int count = 1;
    Mat structure_element = getStructuringElement(MORPH_RECT, Size(7, 7));
    vector<int> target_region_areas;

    vector<string>::iterator i;
    for (string i : sample) {
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
        hist = func::getHistogram(image);
        //cout << hist << endl;
        Mat img_cdf;
        img_cdf = func::cdf(hist) / 255;
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


        /*
        * 将图像分成若干个区域
        */
        SubRegion sub_regions;
        sub_regions = func::area_segment(image, PRE_AREA_NUM);


        /*
        * 将面积第二的区域提取出来
        */
        vector<int> regions_area = sub_regions.area;
        vector<int> regions_area_index = func::range(regions_area.size());
        func::quickSortWithIndex(regions_area, regions_area_index, 0, regions_area.size());
        int ind = regions_area_index[regions_area_index.size() - 2];
        int target_region_value = sub_regions.value[ind];  // 获取到面积第二的区域的值
        target_region_areas.push_back(sub_regions.area[ind]);  // 将该区域的面积记录下来
        Mat compare_mat = Mat::ones(image.size(), image.depth()) * target_region_value;
        Mat diff = compare_mat != image;
        image = diff;


        /*
        * 输出所有子区域区域信息
        */
        cout << "value of subregions: ";
        for (auto i : sub_regions.value)
            cout << i << ' ';
        cout << endl;
        cout << "area of subregions: ";
        for (auto i : sub_regions.area)
            cout << i << ' ';
        cout << endl << endl;

        namedWindow("img" + to_string(count));
        imshow("img" + to_string(count), image);
        count++;
    }

    cout << "area of target regions: ";
    for (auto target_region_area : target_region_areas)
        cout << target_region_area << ' ';

    time_end = clock();
    cout << endl << "program running time: " << double(time_end - time_start) / CLOCKS_PER_SEC << "s";

    waitKey(0);
    destroyAllWindows();
}

void detect::class1Defect2() {

}
