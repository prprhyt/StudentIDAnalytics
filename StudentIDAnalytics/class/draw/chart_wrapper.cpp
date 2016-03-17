#include"chart_wrapper.hpp"

using namespace std;

donuts_chart::donuts_chart(int x, int y, int len,WCHAR rcvdata[],WCHAR msgdata[]) {
	x_ = x;
	y_ = y;
	width_ = len;
	height_ = len;
	radius_ = len / 2;
	wsprintf(target_word_, _T("%s\0"), rcvdata);
	wsprintf(message_, _T("%s\0"), msgdata);
	colors.push_back(RGB(73, 115, 201));
	colors.push_back(RGB(64,197,242));
	colors.push_back(RGB(247, 187, 1));
	colors.push_back(RGB(70, 87, 103));
	colors.push_back(RGB(73, 215, 201));
}

void donuts_chart::set_chart_elements(student_id_data_node *node) {
	student_id_data_tree studnet_id_data_tree_cls;
	wstring temp_wstring = target_word_,temp_wstring_s;
	int index_num = 0;
	map<long, wstring> temp_maps;
	int temp_count = 0;
	int index_multiply = 1;
	elements_name_list_ = studnet_id_data_tree_cls.get_list_of_elements_name_by_word(node, target_word_);
	sum_ = studnet_id_data_tree_cls.get_number_of_student_id_by_word(node, target_word_);

	index_num = temp_wstring.find(_T("?"), index_num);
	for (int i = 0; i < static_cast<int>(log10(static_cast<double>(elements_name_list_.size() - 1)) + 1); ++i) {
		index_multiply *= 10;
	}
	for (int i = 0; i < elements_name_list_.size(); ++i) {
		WCHAR temp_wchar[16];
		if (index_num ==2 || index_num == 6) {
			temp_wstring.replace(index_num, 2, elements_name_list_[i]);
		}
		else {
			temp_wstring.replace(index_num, 1, elements_name_list_[i]);
		}
		wsprintf(temp_wchar, _T("%s"), temp_wstring.c_str());

		chart_elements_[elements_name_list_[i]] = studnet_id_data_tree_cls.get_number_of_student_id_by_word(node, temp_wchar);
		temp_maps[chart_elements_[elements_name_list_[i]]*index_multiply + i] = elements_name_list_[i];//昇順ソート用
	}

	for (map<long, wstring>::iterator it = temp_maps.begin(); it != temp_maps.end(); ++it) {
		long key = it->first;
		wstring val = it->second;
		elements_name_list_[temp_count] = val;//ソートしたものを順に代入
		++temp_count;
	}
	if (index_num==2|| index_num==6) {
		++index_num;
	}
	++index_num;
}

HDC donuts_chart::draw_donuts_chart(HDC hdc) {//TODO:ジャギーが目立つのでGDI+でアンチエイリアスをかける
	double pie_rad=0;
	WCHAR temp_wchar[256];
	RECT rt;
	int txt_height;
	int column_num = 0;
	RECT rt_s;
	const int row_margin = 5;//合計数の表示と要素の表示の間隔

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
	
	for (int i = 1; i < elements_name_list_.size()+1; ++i) {
		wstring temp_inline_chart_wstring=_T("");
		wstring temp_wstring = _T("");
		for (int j = 0; j < elements_name_list_.size(); j+=i) {
			for (int k = 0; k < i; ++k) {
				if (j + k < elements_name_list_.size()) {
					temp_wstring += _T(" ") + elements_name_list_[j + k];
				}
				else {
					break;
				}
			}
			temp_wstring += _T("\n");
		}
		temp_inline_chart_wstring = message_;
		temp_inline_chart_wstring += _T("\n")+to_wstring(sum_)+_T("\n")+ temp_wstring;
		DrawText(hdc, temp_wstring.c_str(), -1, &rt, DT_CALCRECT);//文字の描画領域の矩形を取得
		int temp_txt_height = rt.bottom - rt.top;
		if (radius_ < (temp_txt_height+ row_margin)/2) {
			break;
		}
		int temp_half_max_width = static_cast<int>(sqrt(radius_*radius_ - (temp_txt_height / 2)*(temp_txt_height / 2))+0.5);
		if (temp_half_max_width < (rt.right - rt.left) / 2) {
			break;
		}
		column_num = i;
		rt_s = rt;
	}

	if(column_num>0){
		wsprintf(temp_wchar, _T("%s\nTotal:%d\0"), message_, sum_);
		DrawText(hdc, temp_wchar, -1, &rt, DT_CALCRECT);//文字の描画領域の矩形を取得
		txt_height = rt.bottom - rt.top;
		rt.left = x_;
		rt.right = x_ + width_;
		rt.top = (height_ - (rt_s.bottom- rt_s.top)) / 2 + y_;
		rt.bottom = txt_height + rt.top;
		DrawText(hdc, temp_wchar, -1, &rt, DT_CENTER);
		wstring temp_inline_chart_wstring;
		wstring temp_wstring = _T("");
		RECT temp_rt = {0,0,0,0};
		int text_width = 0;
		int space_width = 0;
		for (int i = 0; i < column_num; ++i) {
			temp_wstring += _T(" ") + elements_name_list_[0 + i];
		}
		DrawText(hdc, temp_wstring.c_str(), -1, &temp_rt, DT_CALCRECT);//文字の描画領域の矩形を取得
		text_width = temp_rt.right - temp_rt.left;
		DrawText(hdc, _T(" "), -1, &temp_rt, DT_CALCRECT);
		space_width = temp_rt.right - temp_rt.left;
		temp_rt = { 0,0,0,0 };
		temp_rt.top = rt.bottom+ row_margin;
		COLORREF old_text_color = SetTextColor(hdc, RGB(255, 255, 255));
		COLORREF old_back_color = SetBkColor(hdc, RGB(0, 0, 0));

		for (int j = 0; j < elements_name_list_.size(); j += column_num) {
			temp_rt.left = (width_ - text_width) / 2 + x_;
			rt = { 0,0,0,0 };
			for (int k = 0; k < column_num; ++k) {
				if (j + k < elements_name_list_.size()) {
					SetBkColor(hdc, colors[j + k]);
					temp_wstring = elements_name_list_[j + k];
					temp_rt.left += rt.right - rt.left + space_width;
					DrawText(hdc, temp_wstring.c_str(), -1, &rt, DT_CALCRECT);
					temp_rt.right= temp_rt.left+rt.right-rt.left;
					temp_rt.bottom = rt.bottom - rt.top + temp_rt.top;
					temp_wstring = elements_name_list_[j + k];
					DrawText(hdc, temp_wstring.c_str(), -1, &temp_rt, DT_CENTER);
				}
				else {
					break;
				}
			}
			temp_rt.top += rt.bottom-rt.top;
		}
		SetTextColor(hdc, old_text_color);
		SetBkColor(hdc, old_back_color);
	}
	else {
		wsprintf(temp_wchar, _T("%s\nTotal:%d\0"), message_, sum_);
		DrawText(hdc, temp_wchar, -1, &rt, DT_CALCRECT);//文字の描画領域の矩形を取得
		txt_height = rt.bottom - rt.top;
		rt.left = x_;
		rt.right = x_ + width_;
		rt.top = (height_ - txt_height) / 2 + y_;
		rt.bottom = (height_ + txt_height) / 2 + y_;
		DrawText(hdc, temp_wchar, -1, &rt, DT_CENTER);
	}

	SelectObject(hdc, oldpen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hWhitePen);
	DeleteObject(hWhiteBrush);

	return hdc;
}

