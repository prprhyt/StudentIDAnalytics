#include"felicalib_sidacs_wrapper.hpp"

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
		std::sprintf(temp_data, "%s", data);//unsigned char -> char
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
}