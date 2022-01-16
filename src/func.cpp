#include "func.h"

#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

/// <summary>
/// generate a vector on [%start, %end), with %num elements.
/// </summary>
/// <param name="start">: start value. </param>
/// <param name="stop">: end value(not included). </param>
/// <param name="num">: elements' numbers of vector. </param>
/// <returns> vetor with int values. </returns>
std::vector<int> func::linspace(float start, float stop, int num) {
    using std::vector;

    float step = (stop - start) / num;
    vector<int> out;
    float temp = start;
    for (int i = 0; i < num; ++i) {
        out.push_back(int(temp));
        temp = temp + step;
    }
    return out;
}


/// <summary>
/// generate a vector range from 0 to num-1.
/// </summary>
/// <param name="num"></param>
/// <returns> vector of int values. </returns>
std::vector<int> func::range(int num) {
    using std::vector;

    vector<int> out(num, 0);
    for (int i = 0; i < num; ++i)
        out[i] = i;

    return out;
}


/// <summary>
/// use quick sort on vector %v, return sorted vector v and sorted index.
/// </summary>
/// <param name="v">: vector going to be sorted. </param>
/// <param name="v_index">: original index of %v, range from 0 to v.size(). </param>
/// <param name="start">: start index of vector v. </param>
/// <param name="end">: end index of vector v. </param>
void func::quickSortWithIndex(std::vector<int>& v, std::vector<int>& v_index, int start, int end) {
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


/// <summary>
/// get normalized histogram.
/// </summary>
/// <param name="img">: input image. </param>
/// <returns>
/// normalized histogram of input image.
/// </returns>
cv::Mat func::getHistogram(const cv::Mat& img) {
    using namespace cv;

    Mat hist = Mat::zeros(Size(256, 1), CV_32F);
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
/// <param name="in_pic_histogram">: histogram of image. </param>
/// <returns>
/// CDF of the image.
/// </returns>
cv::Mat func::cdf(const cv::Mat& in_pic_histogram) {
    using namespace cv;

    int hist_size = in_pic_histogram.total();
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


/// <summary>
/// search region using recursion.
/// </summary>
/// <param name="img">: input image. </param>
/// <param name="x">: x coordinate in row direction. </param>
/// <param name="y">: y coordinate in column direction</param>
/// <param name="value">: new value of region, in order to saperate from origin value. </param>
/// <param name="region_area">: area of region. </param>
/// <returns></returns>
int func::neighborExpand(cv::Mat& img, int x, int y, int value, int region_area) {
    using namespace cv;
    using std::vector;

    img.at<uchar>(x, y) = value;
    region_area += 1;
    vector<int> img_shape = { img.rows, img.cols };
    vector<vector<int>> ind = { {x, y + 1}, {x + 1, y}, {x, y - 1}, {x - 1, y} };
    for (vector<int>i : ind) {
        if ((i[0] < 0) || (i[0] >= img_shape[0]) || (i[1] < 0) || (i[1] >= img_shape[1]))
            continue;
        if (img.at<uchar>(i[0], i[1]) == 0)
            region_area = neighborExpand(img, i[0], i[1], value, region_area);
    }

    return region_area;
}


SubRegion func::areaSegment(cv::Mat& img, int pre_area_num) {
    using namespace cv;
    using std::vector;

    vector<int> region_value = linspace(2, 255, pre_area_num);
    int region_num = 0;
    SubRegion out;
    vector<int> img_shape = { img.rows, img.cols };
    for (int i = 0; i < img_shape[0]; ++i) {
        for (int j = 0; j < img_shape[1]; ++j) {
            if (img.at<uchar>(i, j) == 0) {
                int value = 0;
                int area = 0;
                vector<int> start = { i ,j };
                value = region_value[region_num];
                area = neighborExpand(img, i, j, value, area);
                region_num += 1;
                out.value.push_back(value);
                out.area.push_back(area);
                out.start.push_back(start);
            }
        }
    }
    return out;
}


cv::Mat func::templateGenerate(std::vector<std::string> refer_sample, cv::Range row_wise, cv::Range col_wise, int canny[]) {
    using namespace cv;
    using std::vector;
    using std::string;

    /*
    * 第一步，对每张图像缩放，然后求所有图像的平均
    */
    Mat t;
    float refer_count = 0;
    for (auto i : refer_sample) {
        Mat image = imread(i, 0);
        float scale = (image.rows < image.cols ? image.rows : image.cols) / float(500);
        Size new_size = Size(round(image.cols / scale), round(image.rows / scale));
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
    * 第二步，对图像进行高斯平滑
    */
    GaussianBlur(t, t, Size(3, 3), 1);

    /*
    * 第三步，Canny法提取图像边缘
    */
    Canny(t, t, canny[0], canny[1]);

    return t;
}
