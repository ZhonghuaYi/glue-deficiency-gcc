#include<vector>

#include"roi.h"
#include"func.h"
#include <opencv2/highgui.hpp>

/// <summary>
/// 阈值分割
/// </summary>
/// <param name="image">: 原图像</param>
/// <param name="area_percent">: 灰度阈值以下的灰度所占图像的面积比 </param>
/// <param name="pre_area_num">: 预设的最大区域数量 </param>
/// <param name="structure_element">: 用于形态学计算的结构元素 </param>
/// <returns> 目标区域的面积 </returns>
int roi::thresholdSegment(cv::Mat &image, float area_percent, const cv::Mat &structure_element) {
    using namespace std;
    using namespace cv;

    /*
    * opencv计算直方图的官方方法
    int channels[] = { 0 };
    const int histSize[] = { 256 };
    float range[] = { 0, 256 };
    const float* ranges[] = { range };
    calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
    */

    /*
    * 第一步
    * 在参考图像中，当手动阈值在37时，阈值分割效果明显。于是考虑到灰度小于37的区域大概面积占比是0.3，
    * 于是将图像中灰度值较低的30%区域分割出来。这里利用了cdf，它本身是直方图的累积分布，因此只需要寻找
    * cdf中最接近0.3的位置，其索引即是能够将30%灰度比较低的区域分割出来的阈值
    */
    Mat hist;
    hist = func::getHistogram(image);
    //cout << hist << endl;
    Mat img_cdf;
    img_cdf = func::cdf(hist) / 255;
    int point[2] = {};
    minMaxIdx(abs(img_cdf - area_percent), 0, 0, point, 0);
    int index = point[1];

    /*
    * 第二步
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
    * 第三步
    * 将图像分成若干个区域
    */
    SubRegion sub_regions;
    sub_regions = func::areaSegment(image);
    /*
    * 第四步
    * 将面积第二的区域提取出来
    */
    vector<int> regions_area = sub_regions.area;
    vector<int> regions_area_index = func::range(int(regions_area.size()));
    func::quickSortWithIndex(regions_area, regions_area_index, 0, int(regions_area.size()));
    int ind = regions_area_index[regions_area_index.size() - 2];
    int target_region_value = sub_regions.value[ind];  // 获取到面积第二的区域的值
    int region_area = sub_regions.area[ind];  // 将该区域的面积记录下来
    Mat compare_mat = Mat::ones(image.size(), image.depth()) * target_region_value;
    Mat diff = (compare_mat != image);
    image = diff;

    return region_area;
}

/// <summary>
/// 模板匹配
/// </summary>
/// <param name="image">: 原图像</param>
/// <param name="target_template">: 目标区域的模板 </param>
/// <param name="canny">: Canny法的低阈值和高阈值 </param>
/// <returns> 目标区域与模板的相关系数 </returns>
float roi::templateMatch(cv::Mat &image, const cv::Mat &target_template, std::vector<int> canny, int flag) {
    using namespace std;
    using namespace cv;

    int template_shape[2] = {target_template.rows, target_template.cols};
    /*
    * 第一步，将图像缩放到一个统一的大小（较小边为500像素）
    */
    double scale = float(image.rows < image.cols ? image.rows : image.cols) / float(500);
    Size new_size = Size(int(round(image.cols / scale)), int(round(image.rows / scale)));
    resize(image, image, new_size);

    /*
    * 第二步，对图像进行高斯平滑
    */
    GaussianBlur(image, image, Size(3, 3), 1);
    Mat image_no_edge;
    if (flag == 1)
        image_no_edge = image.clone();

    /*
    * 第三步，Canny法提取图像边缘
    */
    Canny(image, image, canny[0], canny[1]);

    /*
     * 获取模板和图像的三层下取样金字塔
     */
    vector<Mat> t_pyramid = func::gaussianPyramid(target_template, "down", 3);
    vector<Mat> img_pyramid = func::gaussianPyramid(image, "down", 3);

    /*
     * 通过从金字塔上层开始进行模板匹配，找到最匹配的角度，误差在0.2度
     */
    float angle_interval[2] = {-45, 45};  // 模板旋转的角度范围
    int angle_num[3] = {10, 11, 11};  // 对每层模板旋转的角度个数，长度应与t_pyramid相同
    float match_angle = 0;
    for (int i = 0; i < t_pyramid.size(); i++) {
        double max_ccoeff = 0;
        float angle_step = (angle_interval[1] - angle_interval[0]) / (float(angle_num[i]) - 1);
        for (int j = 0; j < angle_num[i]; j++) {
            float angle = angle_interval[0] + float(j) * angle_step;  // 得到角度

            /*
             * 旋转模板
             */
            Mat M = getRotationMatrix2D(Point(t_pyramid[i].cols / 2, t_pyramid[i].rows / 2), angle, 1);
            Mat t;
            warpAffine(t_pyramid[i], t, M, Size(t_pyramid[i].cols, t_pyramid[i].rows));

            /*
             * 匹配区域
             */
            Mat res;
            matchTemplate(img_pyramid[i], t, res, TM_CCOEFF_NORMED); //这里使用归一化的相关系数
            double max_val = 0;
            minMaxLoc(res, nullptr, &max_val, nullptr, nullptr);
            auto CCOEFF = max_val; // 记录此时最匹配区域的相关系数
            if (CCOEFF >= max_ccoeff) {
                max_ccoeff = CCOEFF;
                match_angle = angle;
            }
        }
        if (i < (t_pyramid.size() - 1)) {
            angle_interval[0] = match_angle - angle_step;
            angle_interval[1] = match_angle + angle_step;
        }
    }

//     cout << "most matched angle: " << match_angle << endl;

    /*
    * 通过模板匹配，找到目标区域
    */
    Mat M = getRotationMatrix2D(Point(image.cols / 2, image.rows / 2), -match_angle, 1);
    Mat t;
    warpAffine(image, image, M, Size(image.cols, image.rows));
    Mat res;
    matchTemplate(image, target_template, res, TM_CCOEFF_NORMED);
    double max_val = 0;
    Point max_loc;
    minMaxLoc(res, nullptr, &max_val, nullptr, &max_loc);
    auto CCOEFF = float(max_val); // 记录此时最匹配区域的相关系数
    Point left_top = max_loc; // 最匹配模板的区域的左上角坐标，为宽和高，不是x和y坐标
    if (flag == 0)
        image = image(Rect(left_top.x, left_top.y, template_shape[1], template_shape[0]));
    else if (flag == 1)
        image = image_no_edge(Rect(left_top.x, left_top.y, template_shape[1], template_shape[0]));
    return float(CCOEFF);
}