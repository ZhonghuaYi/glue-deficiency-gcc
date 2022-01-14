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
    * ��ȡ����
    */
    vector<string> REFER_LIST = { "test000.bmp" };
    vector<string> DEFECT_LIST = { "test001.bmp", "test002.bmp" };
    string img_root_path = "../image/";
    vector<string> sample;
    sample.insert(sample.end(), REFER_LIST.begin(), REFER_LIST.end());
    sample.insert(sample.end(), DEFECT_LIST.begin(), DEFECT_LIST.end());

    /*
    * �趨����ʼ����ʱ��
    */
    clock_t time_start, time_end;
    time_start = clock();

    int count = 1;  // ͼ��ļ���
    vector<int> results; // �洢�������жϽ��

    string method = "thresh"; // ʹ�õķָ��
    string f = "ccoeff";  // �������������

    if (method == "thresh") {
        float AREA_PRECENT = 0.3;  // �Ҷ���ֵ�����ռ��
        int PRE_AREA_NUM = 12;  // Ԥ��������������
        int NORMAL_AREA = 420;  // Ŀ��������������

        /*
        * ������̬ѧ����ľ��νṹԪ��
        */
        Mat structure_element = getStructuringElement(MORPH_RECT, Size(7, 7));
        vector<int> target_region_areas;

        vector<string>::iterator i;
        for (string i : sample) {
            Mat image = imread(img_root_path + i, 0);
            
            /*
            * ��ֵ�ָ�
            */
            int region_area = segment::threshold_segment(image, AREA_PRECENT, PRE_AREA_NUM, structure_element);

            /*
            * ���������жϴ������Ƿ�ϸ�resultΪ1��ʾ�ϸ�0��ʾ��ȱ��
            */
            int result = feature::region_area(region_area, NORMAL_AREA);
            results.push_back(result);

            /*
            * ��ʾ���շ�����������ͼ��
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
