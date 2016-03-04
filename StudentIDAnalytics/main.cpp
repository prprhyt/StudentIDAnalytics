#include<Windows.h>
#include"felicalib_wrapper\felicalib_sidacs_wrapper.hpp"
#define _T TEXT


ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass);
BOOL InitInstance(HWND hwnd, HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

//Global variables
felicalib_wrapper flib_wrapper;

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
	switch (msg) {
	case WM_CREATE:
		flib_wrapper.init_felica();
		break;
	case WM_DESTROY:
		flib_wrapper.destroy_felica();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		student_id_details sids;
		hdc = GetDC(hwnd);
		if (!flib_wrapper.read_data(StudentID)) {
			TextOut(hdc, 10, 10, StudentID, lstrlen(StudentID));
			sids.get_student_id_details(StudentID);
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = GetDC(hwnd);

		ReleaseDC(hwnd, hdc);
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}