#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <ctime>

#include "func.h"
#include "data.h"
#include "feature.h"
#include "roi.h"


void threshSegment(cv::Mat image, float area_percent, const cv::Mat &structure_element, int normal_area,
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

void
templateMatch(const cv::Mat &image, const std::vector<cv::Mat> &edge_templates, const std::vector<cv::Mat> &templates,
              std::vector<std::vector<int>> canny, const std::string &f, std::vector<float> thresh) {
    using namespace std;
    using namespace cv;

    clock_t start_time, end_time;
    start_time = clock();

    int t_count = 0;
    for (auto &t: edge_templates) {
        int result = -1;
        if (f == "ccoeff") {
            // 检测
            Mat image_roi = image.clone();
            float CCOEFF = roi::templateMatch(image_roi, t, canny[t_count]);
//            imshow("img" + to_string(t_count), image_roi);
//            waitKey(0);
            cout << "Region " << t_count + 1 << "'s correlation coefficient: " << CCOEFF << endl;
            result = feature::correlation(CCOEFF, thresh[1], thresh[0]);
        } else if (f == "hough") {
            Mat image_roi = image.clone();
            roi::templateMatch(image_roi, t, canny[t_count]);
            if (t_count == 0) {
                Mat drawing = Mat::zeros(image_roi.rows, image_roi.cols, image_roi.depth());
                vector<Vec4i> lines = func::defect1HoughLine(image_roi);
                func::drawLine(drawing, lines);
//                imshow("hough1", drawing);
//                waitKey(0);
                result = feature::defect1Hough(float(image_roi.rows), float(image_roi.cols), lines, 4);
            } else if (t_count == 1) {
                Mat drawing = Mat::zeros(image_roi.rows, image_roi.cols, image_roi.depth());
                vector<Vec4i> lines = func::defect2HoughLine(image_roi);
                func::drawLine(drawing, lines);
//                imshow("hough2", drawing);
//                waitKey(0);
                result = feature::defect2Hough(float(image_roi.rows), float(image_roi.cols), lines, 10);
            }
        }
        else if (f == "sift"){
            bool USE_DES = true;
            vector<KeyPoint> kp_t, kp_img;
            Mat des_t, des_img;
            vector<DMatch> matches;
            if(USE_DES){
                // 检测
                Mat image_roi = image.clone();
                roi::templateMatch(image_roi, t, canny[t_count], 1);
                // 创建sift实例
                Ptr<SIFT> sift = SIFT::create();
                // 模板的sift特征
                sift->detectAndCompute(templates[t_count], Mat() ,kp_t, des_t);
//                // 显示模板的sift特征
//                Mat template_sift;
//                drawKeypoints(templates[t_count], kp_t, template_sift);
//                imshow("template"+ to_string(t_count+1)+"_sift", template_sift);
//                waitKey(0);
                // 图像roi的sift特征
                sift->detectAndCompute(image_roi, Mat() ,kp_img, des_img);
//                //显示图像roi的sift特征
//                Mat img_sift;
//                drawKeypoints(image_roi, kp_img, img_sift);
//                imshow("roi"+ to_string(t_count+1)+"_sift", img_sift);
                // 对图像和模板的sift特征进行匹配
                auto bf = BFMatcher::create(NORM_L2, true);
                if(!kp_img.empty()){
                    bf->match(des_t, des_img, matches);
                    vector<KeyPoint> new_kp_t, new_kp_img;
                    for(auto& m : matches){
                        new_kp_t.push_back(kp_t[m.queryIdx]);
                        new_kp_img.push_back(kp_img[m.trainIdx]);
                    }
                    matches = func::pointLocationMatch(new_kp_t, new_kp_img, 10);
                    kp_t = new_kp_t;
                    kp_img = new_kp_img;
                    Mat match_image;
                    drawMatches(templates[t_count], kp_t, image_roi, kp_img, matches, match_image,
                                Scalar::all(-1), Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::DEFAULT);
                    imshow("match"+ to_string(t_count+1), match_image);
                    waitKey(0);
                }
            }
            else{
                // 检测
                Mat image_roi = image.clone();
                roi::templateMatch(image_roi, t, canny[t_count], 1);
                // 创建sift实例
                Ptr<SIFT> sift = SIFT::create();
                // 获取特征点
                sift->detect(templates[t_count], kp_t);
                sift->detect(image_roi, kp_img);
                // 计算位置相近的特征点
                if(!kp_img.empty()){
                    matches = func::pointLocationMatch(kp_t, kp_img, 10);
                    Mat match_image;
                    drawMatches(templates[t_count], kp_t, image_roi, kp_img, matches, match_image,
                                Scalar::all(-1), Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::DEFAULT);
                    imshow("match"+ to_string(t_count+1), match_image);
                    waitKey(0);
                }
            }
            result = feature::keyPoints(kp_t, kp_img, matches, thresh[1], thresh[0], 1000);
        }
        func::resultExplain(result, t_count + 1);
        t_count++;
    }
    end_time = clock();
    cout << "program running time: " << double(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

    destroyAllWindows();
}

int main() {
    using namespace std;
    using namespace cv;
    Data loader;
    loader.LoadData(1, "template_match");
    int image_count = 1;
    for (auto &i: loader.sample) {
        cout << "-----------------------------" << endl;
        cout << "sample " << image_count << ":" << endl;
        Mat image = imread(i, 0);
//        threshSegment(image, loader.area_percent, loader.structure_element,
//                      loader.normal_area, loader.thresh);
        cout << loader.f << endl;
        templateMatch(image, loader.edge_templates, loader.templates, loader.canny, loader.f, loader.t_thresh);
        image_count++;
    }
    return 0;
}
