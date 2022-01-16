﻿#pragma once
#ifndef _SEGMENT_H_

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

namespace segment {
	int thresholdSegment(cv::Mat& image, float area_percent, int pre_area_num, cv::Mat structure_element);
	float templateMatch(cv::Mat& image, const cv::Mat target_template, int* canny);
}

#endif // !_SEGMENT_H_

