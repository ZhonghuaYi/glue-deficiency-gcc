﻿#pragma once
#ifndef _SEGMENT_H_

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

namespace segment {
	int threshold_segment(cv::Mat& image, float area_percent, int pre_area_num, cv::Mat structure_element);
	float template_match(cv::Mat& image, const cv::Mat target_template, const int* canny);
}

#endif // !_SEGMENT_H_

