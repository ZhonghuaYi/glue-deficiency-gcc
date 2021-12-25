#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    using namespace std;
    using namespace cv;
    string img_path = "../image/test000.bmp";
    Mat image;
    image = imread(img_path, 1);
    namedWindow("img", WINDOW_AUTOSIZE);
    imshow("img", image);
    std::cout << "Hello, World!" << std::endl;
    waitKey(0);
    return 0;
}
