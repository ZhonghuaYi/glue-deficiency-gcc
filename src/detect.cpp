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
        * opencv����ֱ��ͼ�Ĺٷ�����
        int channels[] = { 0 };
        const int histSize[] = { 256 };
        float range[] = { 0, 256 };
        const float* ranges[] = { range };
        calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
        */


        /*
        * �ڲο�ͼ���У����ֶ���ֵ��37ʱ����ֵ�ָ�Ч�����ԡ����ǿ��ǵ��Ҷ�С��37�����������ռ����0.3��
        * ���ǽ�ͼ���лҶ�ֵ�ϵ͵�30%����ָ����������������cdf����������ֱ��ͼ���ۻ��ֲ������ֻ��ҪѰ��
        * cdf����ӽ�0.3��λ�ã������������ܹ���30%�ҶȱȽϵ͵�����ָ��������ֵ
        */
        hist = func::getHistogram(image);
        //cout << hist << endl;
        Mat img_cdf;
        img_cdf = func::cdf(hist) / 255;
        int point[2] = {};
        minMaxIdx(abs(img_cdf - 0.3), 0, 0, point, 0);
        int index = point[1];


        /*
        * �õ���ֵ�󣬶�ͼ�������ֵ�ָȻ��Բ���������Ӧ����ֵ�˲�ƽ����
        * ÿ��ƽ������Ҫͨ�����ͱ����Ӷ���ʴ���壬��ʹ��Ŀ�������ܹ������ױ�����
        */
        medianBlur(image, image, 9);
        dilate(image, image, structure_element);

        resize(image, image, Size(400, 400));
        medianBlur(image, image, 3);
        dilate(image, image, structure_element);

        resize(image, image, Size(70, 70));
        threshold(image, image, index, 255, THRESH_BINARY);


        /*
        * ��ͼ��ֳ����ɸ�����
        */
        SubRegion sub_regions;
        sub_regions = func::area_segment(image, PRE_AREA_NUM);


        /*
        * ������ڶ���������ȡ����
        */
        vector<int> regions_area = sub_regions.area;
        vector<int> regions_area_index = func::range(regions_area.size());
        func::quickSortWithIndex(regions_area, regions_area_index, 0, regions_area.size());
        int ind = regions_area_index[regions_area_index.size() - 2];
        int target_region_value = sub_regions.value[ind];  // ��ȡ������ڶ��������ֵ
        target_region_areas.push_back(sub_regions.area[ind]);  // ��������������¼����
        Mat compare_mat = Mat::ones(image.size(), image.depth()) * target_region_value;
        Mat diff = compare_mat != image;
        image = diff;


        /*
        * �������������������Ϣ
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
