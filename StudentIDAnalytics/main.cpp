#include<Windows.h>
#include"felicalib_wrapper\felicalib_sidacs_wrapper.hpp"
#include"class\draw\chart_wrapper.hpp"


ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass);
BOOL InitInstance(HWND hwnd, HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
DWORD WINAPI pasori_thread_(LPVOID	hwnd);

//Global variables
felicalib_wrapper flib_wrapper;
student_id_data_node* o_node(nullptr);
student_id_data_tree sidt;
donuts_chart dept_chart(50, 50, 300, _T("--??----"),_T("Dept."));

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	MSG msg;
	HWND hwnd=0;

	if (!InitInstance(hwnd,hInstance))
	{
		return FALSE;
	}

	//main message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wcs;

	wcs.style = CS_HREDRAW | CS_VREDRAW;
	wcs.lpfnWndProc = (WNDPROC)WndProc;
	wcs.cbClsExtra = 0;
	wcs.cbWndExtra = 0;
	wcs.hInstance = hInstance;
	wcs.hIcon = NULL;
	wcs.hCursor = 0;
	wcs.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcs.lpszMenuName = 0;
	wcs.lpszClassName = szWindowClass;
	return RegisterClass(&wcs);
}

BOOL InitInstance(HWND hwnd,HINSTANCE hInstance)
{
	WCHAR	*szTitle = _T("StudnetID Analytics");	 //TitleBarText
	WCHAR	szWindowClass[512];		                 //WindowClassName
	lstrcpy(szWindowClass, _T("SIDAcs"));
	MyRegisterClass(hInstance, szWindowClass);

	hwnd = CreateWindow(szWindowClass, szTitle,WS_OVERLAPPEDWINDOW | WS_VISIBLE,CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,NULL, NULL,hInstance, NULL);

	if (!hwnd)
	{
		return FALSE;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	WCHAR StudentID[32];

	//mthread
	static HANDLE	mThread;
	static DWORD TId;

	switch (msg) {
	case WM_CREATE:
		flib_wrapper.init_felica();
		o_node = sidt.create_node("origin");
		sidt.restore_student_id_data(o_node);
		//マルチスレッド
		mThread = CreateThread(NULL, 0, pasori_thread_, hwnd, 0, &TId);
		break;
	case WM_DESTROY:
		CloseHandle(mThread);
		sidt.store_student_id_data(o_node);
		o_node = sidt.delete_node(o_node);
		flib_wrapper.destroy_felica();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:

		break;
	case WM_PAINT:
		hdc = GetDC(hwnd);

		ReleaseDC(hwnd, hdc);
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

DWORD WINAPI pasori_thread_(LPVOID	hwnd){//マルチスレッドで学生IDの読み取り待機
	int zero_is_continue = 0;
	HDC hdc;
	WCHAR StudentID[32];
	WCHAR ID_sum_wchar[32];
	int id_sum_num = 0;
	std::vector<std::wstring> students_id_lists;
	while (!zero_is_continue) {
		//InvalidateRect(static_cast<HWND>(hwnd), NULL, FALSE);
		hdc = GetDC(static_cast<HWND>(hwnd));
		if (!flib_wrapper.read_data(StudentID)) {
			//TextOut(hdc, 10, 10, StudentID, lstrlen(StudentID));
			sidt.add_tree_student_id(o_node, StudentID);
			sidt.add_tree_student_id(o_node, _T("4bjt1289"));
			sidt.add_tree_student_id(o_node, _T("3bjt2194"));
			sidt.add_tree_student_id(o_node, _T("5ajt2296"));
			sidt.add_tree_student_id(o_node, _T("2bji1199"));
			id_sum_num = sidt.get_number_of_student_id_by_word(o_node, _T("????????"));
			students_id_lists = sidt.get_list_of_student_id_by_word(o_node, _T("????????"));
			wsprintf(ID_sum_wchar, _T("%d"), id_sum_num);
			//TextOut(hdc, 10, 30, ID_sum_wchar, lstrlen(ID_sum_wchar));
			/*for (int i = 0; i < students_id_lists.size(); ++i) {
				TextOut(hdc, 10, 50+i*20, students_id_lists[i].c_str(), lstrlen(students_id_lists[i].c_str()));
			}*/
			dept_chart.set_chart_elements(o_node);
			dept_chart.draw_donuts_chart(hdc);
		}
		ReleaseDC(static_cast<HWND>(hwnd), hdc);
		Sleep(100);
	}

	return 0;
}
