#pragma once
#ifndef _DETECT_H_

namespace detect {
	void defect1();
	void defect2();
    void template_match(cv::Mat image, std::vector<cv::Mat>templates, std::vector<int[2]> canny, std::string f, float* thresh);
}

#endif // !_DETECT_H_
