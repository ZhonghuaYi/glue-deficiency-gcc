#include "func.h"

using std::vector;
using namespace cv;

/// <summary>
/// generate a vector on [%start, %end), with %num elements.
/// </summary>
/// <param name="start">: start value. </param>
/// <param name="stop">: end value(not included). </param>
/// <param name="num">: elements' numbers of vector. </param>
/// <returns> vetor with int values. </returns>
vector<int> func::linspace(float start, float stop, int num) {
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
vector<int> func::range(int num) {
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
void func::quickSortWithIndex(vector<int>& v, vector<int>& v_index, int start, int end) {
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
Mat func::getHistogram(const Mat& img) {
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
Mat func::cdf(const Mat& in_pic_histogram) {
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
int func::neighbor_expand(Mat& img, int x, int y, int value, int region_area) {
    img.at<uchar>(x, y) = value;
    region_area += 1;
    vector<int> img_shape = { img.rows, img.cols };
    vector<vector<int>> ind = { {x, y + 1}, {x + 1, y}, {x, y - 1}, {x - 1, y} };
    for (vector<int>i : ind) {
        if ((i[0] < 0) || (i[0] >= img_shape[0]) || (i[1] < 0) || (i[1] >= img_shape[1]))
            continue;
        if (img.at<uchar>(i[0], i[1]) == 0)
            region_area = neighbor_expand(img, i[0], i[1], value, region_area);
    }

    return region_area;
}


SubRegion func::area_segment(Mat& img, int pre_area_num) {
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
                area = neighbor_expand(img, i, j, value, area);
                region_num += 1;
                out.value.push_back(value);
                out.area.push_back(area);
                out.start.push_back(start);
            }
        }
    }
    return out;
}