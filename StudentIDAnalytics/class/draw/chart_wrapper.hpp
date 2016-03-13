#pragma once
#ifndef STUDENT_ID_ANALYTICS_CHART_WRAPPER_HPP
#define STUDENT_ID_ANALYTICS_CHART_WRAPPER_HPP
#define _USE_MATH_DEFINES
#include<math.h>
#include<unordered_map>
#include<vector>
#include"../../felicalib_wrapper/felicalib_sidacs_wrapper.hpp"

class donuts_chart {
private:
	int x_, y_, width_, height_, radius_, center_x_,center_y_;
	int sum_;
	std::vector<std::wstring> elements_name_list_;
	std::unordered_map<std::wstring, int> chart_elements_;
	std::vector<COLORREF> colors;
public:
	donuts_chart(int x, int y, int len);
	void set_chart_elements(student_id_data_node*,WCHAR[]);
	HDC draw_donuts_chart(HDC hdc);

};

#endif
