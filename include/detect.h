#pragma once
#ifndef _DETECT_H_
#include<iostream>
#include<vector>
#include<opencv2/core.hpp>

namespace detect {
	void defect1();
	void defect2();
    void templateMatch(cv::Mat image, const std::vector<cv::Mat>&templates, std::vector<int*> canny, const std::string& f, float* thresh);
}

#endif // !_DETECT_H_
