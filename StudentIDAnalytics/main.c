#include<windows.h>

#define _T TEXT


ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass);
BOOL InitInstance(HWND hwnd, HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

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
	WCHAR	*szTitle = _T("SIDAcs");	 //TitleBarText
	WCHAR	szWindowClass[512];		//WindowClassName
	lstrcpy(szWindowClass, _T("SIDAcs"));
	MyRegisterClass(hInstance, szWindowClass);

	hwnd = CreateWindow(szTitle, szWindowClass,WS_OVERLAPPEDWINDOW | WS_VISIBLE,CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,NULL, NULL,hInstance, NULL);

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
	LPTSTR lptStr = _T("SIDAcs");
	switch (msg) {
	case WM_CREATE:

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		TextOut(hdc, 10, 10, lptStr, lstrlen(lptStr));
		ReleaseDC(hwnd, hdc);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}