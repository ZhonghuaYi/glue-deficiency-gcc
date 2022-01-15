#include "feature.h"

/// <summary>
/// 用检测到的区域与没有缺陷时的区域面积比值判断图像是否有缺陷
/// </summary>
/// <param name="area">: 检测到的区域面积 </param>
/// <param name="normal_area">: 没有缺陷时的区域的面积 </param>
/// <returns></returns>
int feature::regionArea(int area, int normal_area) {
	if (float(area) / normal_area < 0.9)
		return 0;
	else
		return 1;
}

/// <summary>
/// 用相关系数判断图像是否有缺陷
/// </summary>
/// <param name="ccoeff">: 相关系数 </param>
/// <returns></returns>
int feature::correlation(int ccoeff) {
	if (ccoeff < 0.7)
		return 0;
	else
		return 1;
}
