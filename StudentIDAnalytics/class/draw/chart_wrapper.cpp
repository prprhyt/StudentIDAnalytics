#include"chart_wrapper.hpp"

using namespace std;

donuts_chart::donuts_chart(int x, int y, int len) {
	x_ = x;
	y_ = y;
	width_ = len;
	height_ = len;
	radius_ = len / 2;
	colors.push_back(RGB(73, 115, 201));
	colors.push_back(RGB(64,197,242));
	colors.push_back(RGB(247, 187, 1));
	colors.push_back(RGB(70, 87, 103));
	colors.push_back(RGB(73, 215, 201));
}

void donuts_chart::set_chart_elements(student_id_data_node *node,WCHAR rcvdata[]) {
	student_id_data_tree studnet_id_data_tree_cls;
	wstring temp_wstring = rcvdata;
	elements_name_list_ = studnet_id_data_tree_cls.get_list_of_elements_name_by_word(node,rcvdata);
	sum_ = studnet_id_data_tree_cls.get_number_of_student_id_by_word(node, rcvdata);
	int index_num = 0;
	index_num = temp_wstring.find(_T("?"), index_num);
	for (int i = 0; i < elements_name_list_.size(); ++i) {
		WCHAR temp_wchar[16];
		if (index_num ==2 || index_num >= 6) {
			temp_wstring.replace(index_num, 2, elements_name_list_[i]);
		}
		else {
			temp_wstring.replace(index_num, 1, elements_name_list_[i]);
		}
		wsprintf(temp_wchar, _T("%s"), temp_wstring.c_str());

		chart_elements_[elements_name_list_[i]] = studnet_id_data_tree_cls.get_number_of_student_id_by_word(node, temp_wchar);
	}
	if (index_num==2|| index_num==6) {
		++index_num;
	}
	++index_num;
}

HDC donuts_chart::draw_donuts_chart(HDC hdc) {//TODO:ジャギーが目立つのでGDI+でアンチエイリアスをかける
	double pie_rad=0;

	HPEN hWhitePen = static_cast<HPEN>(GetStockObject(WHITE_PEN));
	HPEN oldpen = static_cast<HPEN>(SelectObject(hdc, hWhitePen));

	for (int i = 0; i <elements_name_list_.size(); ++i) {
		int x_begin,x_end,y_begin,y_end;
		double temp_rad = pie_rad;
		double per = static_cast<double>(chart_elements_[elements_name_list_[i]]) / static_cast<double>(sum_);
		pie_rad += 2*per * M_PI;
		x_begin = width_ / 2-radius_*cos(temp_rad-M_PI/2)+x_;
		x_end = width_ / 2-radius_*cos(pie_rad - M_PI / 2)+ x_;
		y_begin = radius_*sin(temp_rad-M_PI / 2)+y_+height_/2;
		y_end = radius_*sin(pie_rad - M_PI / 2) + y_+height_/2;
		HBRUSH hBrush = CreateSolidBrush(colors[i]);
		HBRUSH hOldBrush_s= static_cast<HBRUSH>(SelectObject(hdc, hBrush));
		Pie(hdc, x_, y_, x_+width_, y_+height_, x_begin, y_begin, x_end, y_end);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hOldBrush_s);
		DeleteObject(hBrush);
	}

	HBRUSH hWhiteBrush = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, hWhiteBrush));
	Pie(hdc, x_ + width_*0.2, y_ + height_*0.2, x_ + width_*0.8, y_ + height_*0.8, x_ + width_ / 2, y_ + height_*0.2, x_ + width_ / 2, y_ + height_*0.2);

	SelectObject(hdc, oldpen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hWhitePen);
	DeleteObject(hWhiteBrush);

	return hdc;
}
