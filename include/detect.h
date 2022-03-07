#pragma once
#ifndef _DETECT_H_

namespace detect {
	void defect1();
	void defect2();
    void templateMatch(cv::Mat image, const std::vector<cv::Mat>&templates, std::vector<int[2]> canny, std::string f, float* thresh);
}

#endif // !_DETECT_H_
