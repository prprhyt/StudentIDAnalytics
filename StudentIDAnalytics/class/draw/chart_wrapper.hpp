#pragma once
#ifndef STUDENT_ID_ANALYTICS_CHART_WRAPPER_HPP
#define STUDENT_ID_ANALYTICS_CHART_WRAPPER_HPP
#define _USE_MATH_DEFINES
#include<math.h>
#include<unordered_map>
#include<map>
#include<vector>
#include<string>
#include<random>
#include<windows.h>
#include"../../felicalib_wrapper/felicalib_sidacs_wrapper.hpp"

class donuts_chart {
private:
	int x_, y_, width_, height_, radius_,sum_;
	WCHAR target_word_[16];
	WCHAR message_[256];
	std::vector<std::wstring> elements_name_list_;
	std::unordered_map<std::wstring, int> chart_elements_;
	static std::unordered_map<std::wstring, COLORREF> chart_colors_;
	static std::unordered_map<COLORREF, char> colors_index_;
	static std::unordered_map<int, int> hue_range_index_;
	std::wstring chart_color_index__wstring;
public:
	donuts_chart(int, int, int ,WCHAR[],WCHAR[]);
	void set_chart_elements(student_id_data_node*);
	HDC draw_donuts_chart(HDC);
	HDC draw_elements_details(HDC,HWND);

};

class color_convert {
public:
	COLORREF hsv_to_rgb(int, int, int);
};

class rank_table {
private:
	int x_, y_, width_, height_;
	std::vector<std::wstring> student_rank_list;
	student_id_time_stamp_tree sitst;
public:
	rank_table(int, int, int ,int);
	void set_rank_table(student_id_data_node*,WCHAR[]);
	HDC draw_rank_table(HDC);

};
#endif
