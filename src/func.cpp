#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

#include "func.h"


cv::Mat func::imageResize(const cv::Mat& image, int size){
    cv::Mat out;
    using namespace std;
    float scale = float(min(image.rows, image.cols)) / float(size);
    cv::Size new_size(int(round(image.cols / scale)), int(round(image.rows / scale)));
    cv::resize(image, out, new_size);
    return out;
}


std::vector<int> func::linspace(float start, float stop, int num) {
    using std::vector;

    float step = (stop - start) / float(num);
    vector<int> out;
    float temp = start;
    for (int i = 0; i < num; ++i) {
        out.push_back(int(temp));
        temp = temp + step;
    }
    return out;
}


std::vector<int> func::range(int num) {
    using std::vector;

    vector<int> out(num, 0);
    for (int i = 0; i < num; ++i)
        out[i] = i;

    return out;
}


void func::quickSortWithIndex(std::vector<int> &v, std::vector<int> &v_index, int start, int end) {
    if (start < end) {
        int x = v[start];
        int i = start;
        int j;
        for (j = start + 1; j < end; j++) {
            if (v[j] <= x) {
                i++;
                std::swap(v[i], v[j]);
                std::swap(v_index[i], v_index[j]);
            }
        }

        std::swap(v[i], v[start]);
        std::swap(v_index[i], v_index[start]);
        quickSortWithIndex(v, v_index, start, i);
        quickSortWithIndex(v, v_index, i + 1, end);
    }
}


cv::Mat func::getHistogram(const cv::Mat &img) {
    using namespace cv;

    Mat hist = Mat::zeros(Size(256, 1), CV_32F);
    Mat out;
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            int index = int(img.at<uchar>(i, j));
            hist.at<float>(0, index) += 1;
        }
    }
    out = hist / double(img.total());
    return out;
}


cv::Mat func::cdf(const cv::Mat &in_pic_histogram) {
    using namespace cv;

    int hist_size = int(in_pic_histogram.total());
    Mat transform = Mat::zeros(in_pic_histogram.size(), in_pic_histogram.depth());
    float temp = 0.;
    for (int i = 0; i < hist_size; ++i) {
        temp += in_pic_histogram.at<float>(0, i);
        transform.at<float>(0, i) = temp;
    }
    transform = transform * (hist_size - 1);
    transform.convertTo(transform, CV_32S);
    transform.convertTo(transform, CV_32F);
    return transform;
}


std::vector<std::string> func::referGenerate(const std::string &dir_path) {
    using namespace std;
    namespace fs = std::filesystem;

    vector<string> refer_sample;
    for (const auto &file: fs::directory_iterator(dir_path)) {
        fs::path filename = file.path().filename();
        if (filename.string().substr(0, 5) != "refer")
            continue;
        string file_path = dir_path + "/" + filename.string();
        refer_sample.push_back(file_path);
    }
    return refer_sample;
}


std::vector<std::string> func::sampleGenerate(const std::string &dir_path,
                                              const std::vector<std::string> &sample_list) {
    using namespace std;
    namespace fs = std::filesystem;

    vector<string> refer_sample;
    if (!sample_list.empty()) {
        for (const auto &s: sample_list) {
            string file_path;
            file_path = dir_path;
            file_path += "/" + s;
            refer_sample.push_back(file_path);
        }
    } else {
        for (const auto &file: fs::directory_iterator(dir_path)) {
            fs::path filename = file.path().filename();
            if (filename.string().substr(0, 6) != "sample")
                continue;
            string file_path = dir_path + "/" + filename.string();
            refer_sample.push_back(file_path);
        }
    }

    return refer_sample;
}


int func::neighborExpand(cv::Mat &img, int x, int y, int value, int region_area) {
    using namespace cv;
    using std::vector;

    img.at<uchar>(x, y) = value;
    region_area += 1;
    vector<int> img_shape = {img.rows, img.cols};
    vector<vector<int>> ind = {{x,     y + 1},
                               {x + 1, y},
                               {x,     y - 1},
                               {x - 1, y}};
    for (vector<int> i: ind) {
        if ((i[0] < 0) || (i[0] >= img_shape[0]) || (i[1] < 0) || (i[1] >= img_shape[1]))
            continue;
        if (img.at<uchar>(i[0], i[1]) == 0)
            region_area = neighborExpand(img, i[0], i[1], value, region_area);
    }

    return region_area;
}


SubRegion func::areaSegment(cv::Mat &img) {
    using namespace cv;
    using std::vector;

    int region_value = 1;
    int region_num = 0;
    SubRegion out;
    vector<int> img_shape = {img.rows, img.cols};
    for (int i = 0; i < img_shape[0]; ++i) {
        for (int j = 0; j < img_shape[1]; ++j) {
            if (img.at<uchar>(i, j) == 0) {
                int area = 0;
                vector<int> start = {i, j};
                area = neighborExpand(img, i, j, region_value, area);
                region_value += 1;
                region_num += 1;
                out.value.push_back(region_value);
                out.area.push_back(area);
                out.start.push_back(start);
            }
        }
    }
    return out;
}


std::vector<cv::Mat> func::templateGenerate(const std::vector<std::string> &refer_sample, cv::Range row_wise, cv::Range col_wise,
                               const std::string &flag, int canny[], int thresh) {
    using namespace cv;
    using std::vector;
    using std::string;

    std::vector<cv::Mat> out;
    /*
    * 第一步，对每张图像缩放，然后求所有图像的平均
    */
    Mat t;
    float refer_count = 0;
    for (const auto &i: refer_sample) {
        Mat image = imread(i, 0);
        double scale = float(image.rows < image.cols ? image.rows : image.cols) / float(500);
        Size new_size = Size(int(round(image.cols / scale)), int(round(image.rows / scale)));
        resize(image, image, new_size);
        image = image(row_wise, col_wise);
        image.convertTo(image, CV_32F);
        if (refer_count == 0)
            t = Mat::zeros(image.size(), image.depth());

        t = t + image;
        refer_count += 1;
    }

    t = t / refer_count;
    t.convertTo(t, CV_8U);

    /*
    * 对图像进行平滑
    */
    medianBlur(t, t, 5);
    out.push_back(t);

    if (flag == "canny") {
        /*
        * Canny法提取图像边缘
        */
        Canny(t, t, canny[0], canny[1]);
    } else if (flag == "thresh") {
        threshold(t, t, thresh, 255, THRESH_BINARY);
    }

    out.push_back(t);
    return out;
}


std::vector<cv::Mat> func::gaussianPyramid(cv::Mat image, const std::string &flag, int num) {
    using namespace cv;
    using namespace std;
    vector<Mat> pyramid;
    pyramid.push_back(image);
    if (flag == "up") {
        for (int i = 0; i < num - 1; i++) {
            pyrUp(image, image);
            pyramid.push_back(image);
        }
    } else if (flag == "down") {
        for (int i = 0; i < num - 1; i++) {
            pyrDown(image, image);
            pyramid.push_back(image);
        }
    }
    vector<Mat> temp(pyramid);
    for (int i = 0; i < pyramid.size(); i++)
        pyramid[i] = temp[temp.size() - i - 1];
    return pyramid;
}


void func::resultExplain(int result, int n) {
    std::string message = "Region " + std::to_string(n);
    if (result == 0)
        message = message + ": has defect.";
    else if (result == 1)
        message = message + ": is normal.";
    else if (result == 2)
        message = message + ": has not target region.";
    else
        message = message + ": wrong result code.";

    std::cout << message << std::endl;
}
