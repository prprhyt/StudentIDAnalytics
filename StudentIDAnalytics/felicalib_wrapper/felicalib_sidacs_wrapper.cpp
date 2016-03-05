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
	if(NULL==new_node){
		exit(1);
	}
	new_node->child_node_num = 0;
	sprintf(new_node->label, "%s", label);
	return new_node;
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
		++(sids.count);
		add_tree_node(node->nodes[temp_index_num], sids);
	}
	return 0;
}

void student_id_data_tree::add_personal_num_list(student_id_data_node *node, char label[]) {
	//TODO:学生IDの下2桁を連結リストかVectorで管理する
}

void student_id_data_tree::delete_data(student_id_data_node *node) {
	for (int i = 0; i < node->child_node_num; ++i) {
		delete_data(node->nodes[i]);
	}
	vector<student_id_data_node*>().swap(node->nodes);
	delete node;
}

void student_id_data_tree::add_tree_student_id(student_id_data_node *node,WCHAR rcvdata[]) {
	student_id_details sids;
	char temp_char[6][16];
	int node_num = 0;
	sids.get_student_id_details(rcvdata);
	sids.count = 0;
	add_tree_node(node, sids);
}
