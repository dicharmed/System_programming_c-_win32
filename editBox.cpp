// 2.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "2.h"
#include <commdlg.h>
#include <commctrl.h>
#include <fstream>
#define MAX_LOADSTRING 100
// Global Variables:
TBBUTTON tbb[] =
{ { STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 }};
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Dialog1(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: Place code here.	MSG msg;	HACCEL hAccelTable;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))	{		return FALSE;	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2));
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))	{
	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))		{
		TranslateMessage(&msg);
DispatchMessage(&msg);		}	}	return (int) msg.wParam;}
//  FUNCTION: MyRegisterClass()//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance){	WNDCLASSEX wcex;	wcex.cbSize = sizeof(WNDCLASSEX);	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY2);
	wcex.lpszClassName	= szWindowClass;	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	return RegisterClassEx(&wcex);}
//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window
//   COMMENTS://  In this function, we save the instance handle in a global variable and
//        create and display the main program window.BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){   HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)   {      return FALSE;   }
   ShowWindow(hWnd, nCmdShow);   UpdateWindow(hWnd);   return TRUE;}
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)//  PURPOSE:  Processes messages for the main window.//  WM_COMMAND	- process the application menu//  WM_PAINT	- Paint the main window//  WM_DESTROY	- post a quit message and return
VOID StatusOut(HWND hStatus, int count, TCHAR *str){
	TCHAR text[256];
	_stprintf_s(text, _T("Строк: %d"), count);
	SendMessage(hStatus, SB_SETTEXT, (WPARAM)0, (LPARAM)text);
	SendMessage(hStatus, SB_SETTEXT, (WPARAM)1, (LPARAM)str);}
static int spin = 20;// описали на глобальном уровне переменную spin
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	static OPENFILENAME file;	static int n, sx, sy;	static HWND hEdit, hWndToolBar, hWndStatusBar;
	RECT rt;	PAINTSTRUCT ps;	HDC hdc;	TCHAR str[256];
	int m, k, aWidths[2];	static TCHAR name[256];	char szText[0x7fff];
	std::ifstream in;	std::ofstream out;	switch (message)	{
	case WM_PAINT:
		SetRect(&rt, 0, 0, 100, 100);
		hdc = BeginPaint(hWnd, &ps);
		_stprintf_s(str, _T("spin\t= %d"),			spin);		DrawText(hdc, str, _tcslen(str), &rt, DT_LEFT | DT_EXPANDTABS);
		EndPaint(hWnd, &ps);		break;
	case WM_CREATE:
		InitCommonControls();
		hWndToolBar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | CCS_TOP,
			2, 0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, tbb, 3, 0, 0, 0, 0,
			sizeof(TBBUTTON));
		hEdit = CreateWindow(WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL |
			WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			0, 0, 0, 0, hWnd, (HMENU)1, hInst, NULL);
		file.lStructSize = sizeof(OPENFILENAME);
		file.hInstance = hInst;
		file.lpstrFilter = _T("Text\0 *.txt\0Все файлы\0 *.*");
		file.lpstrFile = name;		file.nMaxFile = 256;
		file.lpstrInitialDir = _T(".\\");		file.lpstrDefExt = _T("txt");
		hWndStatusBar = CreateWindow(STATUSCLASSNAME, NULL, WS_CHILD |
WS_VISIBLE, 0, 0, 0, 0, hWnd, NULL, hInst, NULL); break;case WM_SIZE:
	sx = LOWORD(lParam);//ширина окна sy = HIWORD(lParam);//высота окна
		aWidths[0] = 100;	aWidths[1] = sx;// Поделим строку состояния на две части
		GetWindowRect(hWndToolBar, &rt);// вычислили высоту панели инструментов	
m = rt.bottom - rt.top;
		SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
		GetWindowRect(hWndStatusBar, &rt);	k = rt.bottom - rt.top;
		MoveWindow(hWndStatusBar, 0, sy - k, sx, sy, TRUE);//переопределяем позицию окна
SendMessage(hWndStatusBar, SB_SETPARTS, (WPARAM)2, (LPARAM)aWidths);//возвращаем результат
		StatusOut(hWndStatusBar, n, name);// Вывод в строку состояния
		MoveWindow(hEdit, 0, m, sx, sy - m - k, TRUE);
		UpdateWindow(hEdit);обновляем клиентскую область
		SetFocus(hEdit);// Назначает фокус ввода окну
		return 0;	case WM_COMMAND:
		switch (LOWORD(wParam))		{
		case IDM_ABOUT:	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);//создаем диалоговое окно
			break;		case ID_DIALOG:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog1);
			break;		case ID_FILE_NEW:
szText[0] = '\0';// прочитать файл как двоичный в буфер szText
	SetWindowTextA(hEdit, szText);// поместим текст в виде строки в окно редактирования
			StatusOut(hWndStatusBar, 0, _T(""));			break;
		case ID_FILE_OPEN:
			file.lpstrTitle = _T("Открыть файл для чтения");
			file.Flags = OFN_HIDEREADONLY;
			if (!GetOpenFileName(&file)) return 1;
in.open(name, std::ios::binary);in.read(szText, 0x7fff);// открываем файл как двоичный и читаем блоком не более  0x7fff  символов.
			if ((m = in.gcount()) == 0x7fff)			{
				MessageBox(hWnd, _T("Слишком большой файл"),
		_T("Edit"), MB_OK | MB_ICONSTOP);	in.close();return 0;			}
			szText[m] = '\0';in.close();		SetWindowTextA(hEdit, szText);
			n = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
			StatusOut(hWndStatusBar, n, name); break;
		case ID_FILE_SAVE:
			file.lpstrTitle = _T("Открыть файл для записи");
			file.Flags = OFN_NOTESTFILECREATE | OFN_HIDEREADONLY;
			if (!GetSaveFileName(&file)) return 1;
out.open(name, std::ios::binary);// открываем двоичный файл для записи, но метод для записи блока требует задания его размера, поэтому при копировании текста из окна редактирования в буфер функцией  GetWindowText()  сохраняем его размер  m в байтах
			m = GetWindowTextA(hEdit, szText, 0x7fff);
			out.write(szText, m);out.close();
			n = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
			StatusOut(hWndStatusBar, n, name);break;
		case IDM_EXIT: DestroyWindow(hWnd); break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);	}break;
	case WM_DESTROY:
	PostQuitMessage(0);		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);	}	return 0;}
INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
static HWND hSpin, hBuddy;// функции диалогового окна дескриптор спина.Дескриптор "приятеля"
	switch (message)	{
	case WM_INITDIALOG:
	hSpin = GetDlgItem(hDlg,IDC_SPIN1 );// выводим значение функции
	hBuddy = GetDlgItem(hDlg, IDC_EDIT1);// выводим значение функции
SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)hBuddy, 0);// посылаем  сообщение UDM_SETBUDDY, в wParam передать дескриптор "приятеля"
SendMessage(hSpin, UDM_SETRANGE, 0, 100);// устанавливает диапазон изменения счетчика
	SendMessage(hSpin, UDM_SETPOS, 0, spin);// устанавливает позицию спина
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
	spin = SendMessage(hSpin, UDM_GETPOS, 0, 0);// Считываем состояние наборного счетчика
		InvalidateRect(GetParent(hDlg), NULL, 1);// перерисовываем главное окно функцией
		case IDCANCEL:			EndDialog(hDlg, 0);return TRUE;
		default: return FALSE;		}
	default: return FALSE;	}	return FALSE;}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	UNREFERENCED_PARAMETER(lParam);	switch (message)	{	case WM_INITDIALOG:
		return (INT_PTR)TRUE;	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;		}
		break;}	return (INT_PTR)FALSE;}
