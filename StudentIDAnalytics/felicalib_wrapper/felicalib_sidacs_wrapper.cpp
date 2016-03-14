#include"felicalib_sidacs_wrapper.hpp"

using namespace std;

int felicalib_wrapper::init_felica() {
	p = pasori_open(NULL);
	if (!p) {
		//TODO:エラー用のメソッドを作成する(MessageBoxとログの出力)
		MessageBox(NULL, _T("PaSoRi open failed.\n\rQuits the app."),_T("Error"),MB_OK);
		exit(1);
	}
	pasori_init(p);
	return 0;
}

int felicalib_wrapper::read_data(WCHAR rcvdata[]) {
	felica *f2;
	int timeoutcount = 0;
	static uint8 beforeIDm[8] = { '\0' };
	char temp_data[16];
	while (timeoutcount < 60) {//数値*(1/2)秒でタイムアウト(およそ)
		f = felica_polling(p, POLLING_ANY, 0, 0);

		if (f) {
			break;
		}

		Sleep(500);

		timeoutcount++;

	}

	if (!f) {
		MessageBox(NULL, _T("Polling card failed."), _T("Error"), MB_OK);
		return -1;
	}

	if (memcmp(beforeIDm, (f)->IDm, 8) != 0) {//前回IDmと比較
		memcpy(beforeIDm, (f)->IDm, 8);
		f = felica_enum_systemcode(p);
		if (!f)
		{
			MessageBox(NULL, _T("Reading system code failed."), _T("Error"), MB_OK);
			return -1;
		}


		f2 = felica_enum_service(p, N2HS((f)->system_code[0]));
		if (!f2)
		{
			MessageBox(NULL, _T("Reading system code failed."), _T("Error"), MB_OK);
			felica_free(f2);
			return -1;
		}

		uint16 service = f2->service_code[3];
		uint8 data[16] = "";
		if (felica_read_without_encryption02(f2, service, 0, (uint8)0, data)) {
			MessageBox(NULL, _T("data is not available."), _T("Error"), MB_OK);
			felica_free(f2);
			return -1;
		}
		sprintf(temp_data, "%s", data);//unsigned char -> char
		mbstowcs(rcvdata, temp_data, sizeof(temp_data));//char -> WCHAR
		felica_free(f2);
	}else {
		return -1;
	}
	return 0;
}

void felicalib_wrapper::destroy_felica() {
	felica_free(f);
	pasori_close(p);
	return;
}

int student_id_details::get_student_id_details(WCHAR rcvdata[]) {
	char temp_data[16];
	char temp_char[3];
	int j = 0;
	wcstombs(temp_data, rcvdata, sizeof(rcvdata)*sizeof(WCHAR));
	temp_data[8] = '\0';
	enter_ad = temp_data[0] - '0';
	enter_semester = temp_data[1];
	strncpy(dept, temp_data + 2, 2);
	dept[2] = '\0';
	class_num = temp_data[4] - '0';
	seminar_num = temp_data[5] - '0';
	strncpy(temp_char, temp_data + 6, 2);
	temp_char[2] = '\0';
	personal_num = atoi(temp_char);

	for (int i = 0; i < 8; ++i) {
		if(i == 3 || i == 7){
			continue;
		}
		if (i == 2 || i == 6) {
			strncpy(temp_char, temp_data + i, 2);
			sprintf(details_char[j], "%s", temp_char);
			++j;
		}else {
			sprintf(details_char[j], "%c", temp_data[i]);
			++j;
		}
	}
	return 0;
}

student_id_data_node* student_id_data_tree::create_node(char label[]) {
	student_id_data_node* new_node = new student_id_data_node;
	if(nullptr==new_node){
		exit(1);
	}
	new_node->child_node_num = 0;
	sprintf(new_node->label, "%s", label);
	return new_node;
}

void student_id_data_tree::add_personal_num_list(student_id_data_node *node, int label) {
	vector<int>::iterator itror = lower_bound(node->personal_id.begin(), node->personal_id.end(), label);
	node->personal_id.insert(itror, label);
	//重複削除
	node->personal_id.erase(unique(node->personal_id.begin(), node->personal_id.end()), node->personal_id.end());
	return;
}

int student_id_data_tree::add_tree_node(student_id_data_node *node, student_id_details sids) {
	int child_node_num_temp = node->child_node_num;
	int temp_index_num = -1;
	for (int i = 0; i < child_node_num_temp; ++i) {
		if (!strcmp(node->nodes[i]->label, sids.details_char[sids.count])) {
			temp_index_num = i;//ラベルがすでに存在したら添え字を代入
			break;
		}
	}
	if (temp_index_num < 0) {
		node->nodes.push_back(create_node(sids.details_char[sids.count]));
		node->child_node_num = ++child_node_num_temp;
		temp_index_num = child_node_num_temp - 1;
	}
	if (sids.count < 5) {
		if ((sids.count)++ < 4) {
			add_tree_node(node->nodes[temp_index_num], sids);
		}
		else {
			add_personal_num_list(node->nodes[temp_index_num], sids.personal_num);
		}
	}
	return 0;
}

student_id_data_node* student_id_data_tree::delete_node(student_id_data_node *node) {
	if (nullptr == node) {
		return nullptr;
	}
	for (int i = 0; i < node->child_node_num; ++i) {
		delete_node(node->nodes[i]);
	}
	vector<student_id_data_node*>().swap(node->nodes);
	vector<int>().swap(node->personal_id);
	delete node;
	node = nullptr;
	return nullptr;
}

void student_id_data_tree::add_tree_student_id(student_id_data_node *node,WCHAR rcvdata[]) {
	if (lstrlen(rcvdata) != 8) {
		return;
	}
	student_id_details sids;
	sids.get_student_id_details(rcvdata);
	sids.count = 0;
	add_tree_node(node, sids);
	return;
}

int student_id_data_tree::count_student_id_from_tree(student_id_data_node *node, student_id_details sids) {
	int child_node_num_temp = node->child_node_num;
	int sum_num = 0;
	int temp_sids_count = sids.count;
	for (int i = 0; i < child_node_num_temp; ++i) {
		if (!strcmp("?", sids.details_char[sids.count])) {
			++(sids.count);
			sum_num += count_student_id_from_tree(node->nodes[i], sids);
			sids.count = temp_sids_count;
		}else if (!strcmp(node->nodes[i]->label, sids.details_char[sids.count])) {
			if ((sids.count)++ <= 4) {
				sum_num += count_student_id_from_tree(node->nodes[i], sids);
			}
			sids.count = temp_sids_count;
			break;
		}
	}

	if (sids.count > 4) {
		if (!strcmp("?", sids.details_char[sids.count])) {
			sum_num += (node->personal_id.size());
		}else {
			if (binary_search(node->personal_id.begin(), node->personal_id.end(), atoi(sids.details_char[sids.count]))) {
				++sum_num;
			}
		}
	}
	sids.count = temp_sids_count;


	return sum_num;
}

int student_id_data_tree::get_number_of_student_id_by_word(student_id_data_node *node,WCHAR rcvdata[]) {
	student_id_details sids;
	sids.count = 0;
	int sum_num = 0;
	int data_len = lstrlen(rcvdata);
	if (data_len < 8) {
	
	}else if (8 == data_len) {
		sids.get_student_id_details(rcvdata);
		if (sids.details_char[2][0] == '?' || sids.details_char[2][1] == '?') {
			sprintf(sids.details_char[2], "%s", "?\0");
		}
		if (sids.details_char[5][0] == '?' || sids.details_char[5][1] == '?') {
			sprintf(sids.details_char[5], "%s", "?\0");
		}
		if (sids.details_char[2][0] == '-' || sids.details_char[2][1] == '-') {
			sprintf(sids.details_char[2], "%s", "-\0");
		}
		if (sids.details_char[5][0] == '-' || sids.details_char[5][1] == '-') {
			sprintf(sids.details_char[5], "%s", "-\0");
		}
		for (int i = 0; i < 6; ++i) {
			if (!strcmp("-", sids.details_char[i])) {
				sprintf(sids.details_char[i], "%s", "?\0");
			}
		}
		sum_num = count_student_id_from_tree(node, sids);
	}else {

	}
	return sum_num;
}

vector<wstring> student_id_data_tree::enumerate_student_id_from_tree(student_id_data_node *node, student_id_details sids){
	int child_node_num_temp = node->child_node_num;
	vector<wstring> students_id_list;
	int temp_sids_count = sids.count;
	for (int i = 0; i < child_node_num_temp; ++i) {
		vector<wstring> temp_studnets_id;
		if (!strcmp("?", sids.details_char[sids.count])) {
			++(sids.count);
			temp_studnets_id = enumerate_student_id_from_tree(node->nodes[i], sids);
			for (int j = 0; j < temp_studnets_id.size(); ++j) {
					WCHAR temp_wchar[16];
					mbstowcs(temp_wchar, node->nodes[i]->label, sizeof(node->nodes[i]->label));
					temp_studnets_id[j].insert(0, temp_wchar);
			}
			copy(temp_studnets_id.begin(), temp_studnets_id.end(), back_inserter(students_id_list));
			sids.count = temp_sids_count;
		}
		else if (!strcmp(node->nodes[i]->label, sids.details_char[sids.count])) {
			if ((sids.count)++ <= 4) {
				temp_studnets_id = enumerate_student_id_from_tree(node->nodes[i], sids);
				for (int j = 0; j < temp_studnets_id.size(); ++j) {
					WCHAR temp_wchar[16];
					mbstowcs(temp_wchar, node->nodes[i]->label, sizeof(node->nodes[i]->label));
					temp_studnets_id[j].insert(0, temp_wchar);
				}
				copy(temp_studnets_id.begin(), temp_studnets_id.end(), back_inserter(students_id_list));
			}
			sids.count = temp_sids_count;
			break;
		}
		vector<wstring>().swap(temp_studnets_id);
	}

	if (sids.count > 4) {
		if (!strcmp("?", sids.details_char[sids.count])) {
			for (int i = 0; i < node->personal_id.size(); ++i) {
				WCHAR temp_wchar[16];
				wsprintf(temp_wchar, _T("%d\0"),node->personal_id[i]);
				students_id_list.push_back(temp_wchar);
			}
		}
		else {
			if (binary_search(node->personal_id.begin(), node->personal_id.end(), atoi(sids.details_char[sids.count]))) {
				WCHAR temp_wchar[16];
				mbstowcs(temp_wchar, sids.details_char[sids.count], sizeof(sids.details_char[sids.count]));
				students_id_list.push_back(temp_wchar);
			}
		}
	}

	return students_id_list;
}

vector<wstring> student_id_data_tree::get_list_of_student_id_by_word(student_id_data_node *node, WCHAR rcvdata[]) {
	student_id_details sids;
	sids.count = 0;
	vector<wstring> students_id_list;
	int data_len = lstrlen(rcvdata);
	if (data_len < 8) {

	}
	else if (8 == data_len) {
		sids.get_student_id_details(rcvdata);
		if (sids.details_char[2][0] == '?' || sids.details_char[2][1] == '?') {
			sprintf(sids.details_char[2], "%s", "?\0");
		}
		if (sids.details_char[5][0] == '?' || sids.details_char[5][1] == '?') {
			sprintf(sids.details_char[5], "%s", "?\0");
		}
		students_id_list = enumerate_student_id_from_tree(node, sids);
	}
	else {

	}
	return students_id_list;
}

vector<wstring> student_id_data_tree::enumerate_elements_from_tree(student_id_data_node *node, student_id_details sids) {
	int child_node_num_temp = node->child_node_num;
	vector<wstring> students_id_list;
	int temp_sids_count = sids.count;
	for (int i = 0; i < child_node_num_temp; ++i) {
		vector<wstring> temp_studnets_id;
		if (!strcmp("-", sids.details_char[sids.count])) { //Skip
			++(sids.count);
			temp_studnets_id = enumerate_elements_from_tree(node->nodes[i], sids);
			copy(temp_studnets_id.begin(), temp_studnets_id.end(), back_inserter(students_id_list));
			sids.count = temp_sids_count;
		}
		else if (!strcmp("?", sids.details_char[sids.count])) {
			node->nodes[i]->label;
			WCHAR temp_wchar[16];
			mbstowcs(temp_wchar, node->nodes[i]->label, sizeof(node->nodes[i]->label));
			students_id_list.push_back(temp_wchar);
		}
		else if (!strcmp(node->nodes[i]->label, sids.details_char[sids.count])) {
			if ((sids.count)++ <= 4) {
					temp_studnets_id = enumerate_elements_from_tree(node->nodes[i], sids);
					copy(temp_studnets_id.begin(), temp_studnets_id.end(), back_inserter(students_id_list));
			}
			sids.count = temp_sids_count;
			break;
		}
		vector<wstring>().swap(temp_studnets_id);
	}

	if (sids.count > 4) {
		if (strcmp("-", sids.details_char[sids.count])) {
			if (!strcmp("?", sids.details_char[sids.count])) {
				for (int i = 0; i < node->personal_id.size(); ++i) {
					WCHAR temp_wchar[16];
					wsprintf(temp_wchar, _T("%d\0"), node->personal_id[i]);
					students_id_list.push_back(temp_wchar);
				}
			}
		}
	}
	students_id_list.erase(unique(students_id_list.begin(), students_id_list.end()), students_id_list.end());
	return students_id_list;
}
vector<wstring> student_id_data_tree::get_list_of_elements_name_by_word(student_id_data_node *node, WCHAR rcvdata[]) {
	student_id_details sids;
	sids.count = 0;
	vector<wstring> elements_name_list;
	int data_len = lstrlen(rcvdata);
	if (data_len < 8) {

	}
	else if (8 == data_len) {
		sids.get_student_id_details(rcvdata);
		if (sids.details_char[2][0] == '?' || sids.details_char[2][1] == '?') {
			sprintf(sids.details_char[2], "%s", "?\0");
		}
		if (sids.details_char[5][0] == '?' || sids.details_char[5][1] == '?') {
			sprintf(sids.details_char[5], "%s", "?\0");
		}
		if (sids.details_char[2][0] == '-' || sids.details_char[2][1] == '-') {
			sprintf(sids.details_char[2], "%s", "-\0");
		}
		if (sids.details_char[5][0] == '-' || sids.details_char[5][1] == '-') {
			sprintf(sids.details_char[5], "%s", "-\0");
		}
		elements_name_list = enumerate_elements_from_tree(node, sids);
	}
	else {

	}
	return elements_name_list;
}

int student_id_data_tree::store_student_id_data(student_id_data_node *node) {
	char dir_path[] = "data\\";
	char file_path[MAX_PATH];
	vector<wstring> students_id_list;
	FILE *fp;
	if (FALSE == PathIsDirectoryA(dir_path)) {
		if (FALSE == MakeSureDirectoryPathExists(dir_path)) {
			return -1;
		}
	}
	sprintf(file_path, "%sstudent_id.txt", dir_path);
	if (fopen_s(&fp, file_path, "w") != 0) {
		return -1;
	}
	students_id_list = get_list_of_student_id_by_word(node, _T("????????"));
	for (int i = 0; i < students_id_list.size(); ++i) {
		fwprintf_s(fp, _T("%s\n"),students_id_list[i].c_str());
	}
	fclose(fp);
	return 0;
}

int student_id_data_tree::restore_student_id_data(student_id_data_node *node) {
	char dir_path[] = "data\\";
	char file_path[MAX_PATH];
	WCHAR read_line[MAX_PATH];
	FILE *fp;

	sprintf(file_path, "%sstudent_id.txt", dir_path);
	if (fopen_s(&fp, file_path, "r") != 0) {
		return -1;
	}

	while(NULL != fgetws(read_line, MAX_PATH, fp)) {
		wstring temp_wstring = read_line;
		int index_n = temp_wstring.find(_T("\n"));
		temp_wstring.replace(index_n, 1, _T("\0"));
		wsprintf(read_line, _T("%s\0"), temp_wstring.c_str());
		add_tree_student_id(node, read_line);
	}
	fclose(fp);
	return 0;
}