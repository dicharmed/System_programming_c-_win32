#include "stdafx.h"
#include "буксир.h"
#include <fstream>
#include <commdlg.h>
#include <vector>
#include <string>
#include <commctrl.h>
static int sx, sy;
const int SCALE = 1200;
#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,                   LPTSTR    lpCmdLine, int  nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: Place code here.	MSG msg;	HACCEL hAccelTable;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))	{
		return FALSE;	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);DispatchMessage(&msg); }}
	return (int) msg.wParam;}
//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
//  COMMENTS:
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
ATOM MyRegisterClass(HINSTANCE hInstance){
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;	wcex.cbClsExtra		= 0;	wcex.cbWndExtra		= 0;	wcex.hInstance		= hInstance;	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY);	wcex.lpszClassName	= szWindowClass;	wcex.hIconSm= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);}
//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window//   COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow){
   HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)   {      return FALSE;   }
   ShowWindow(hWnd, nCmdShow);   UpdateWindow(hWnd);   return TRUE;}
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
const int MARK = 6;const int scaleX = 8; //Метки по оси xconst int scaleY = 4; //Метки по оси y
const int indent = 25; //Отступ для вывода меток оси хstruct DOUDLE_POINT { double x, y; };
void DcInLp(POINT &point){	point.x = point.x* SCALE / sx;	point.y = SCALE - point.y* SCALE / sy;}void transform(HDC& hdc){
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, SCALE, -SCALE, NULL);
	SetViewportExtEx(hdc, sx, sy, NULL);	SetViewportOrgEx(hdc, 0, sy, NULL);}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){	PAINTSTRUCT ps;	HDC hdc;	TCHAR s[20];
	static TCHAR nameG1[] = _T("1.0");
	static TCHAR nameG2[] =_T("2.0");
	static TCHAR nameG3[] =_T("3.0");	static TCHAR nameG4[] =_T("4.0");
	static TCHAR nameG5[] =_T("5.0");
	static HPEN hDash, hBezier;	static HBRUSH hRect, hSel;	static POINT pt[20];
	static POINT point;	RECT rt;static POINT *pt1;
	static int count, index; static bool capture;	int i;	std::ifstream in;	std::ofstream out;
	static double max_x, max_y, min_x, min_y;	int  x, y;	
TCHAR K1[11]=_T("0123456789");	TCHAR K2[11]=_T("0123456789");static TCHAR POT[]=_T("4.0;5.0");static TCHAR POT2[]=_T("3.0;4.0");static TCHAR POT0[]=_T("1.0;2.0");
static TCHAR POT4[]=_T("0.0;0.0");	DOUDLE_POINT t;
	double z, hx, hy;	static DOUDLE_POINT *xy;	switch (message)	{
	case WM_CREATE:		in.open("dat.txt");		if (in.fail())		{
		MessageBox(hWnd, _T("Файл dat.txt не найден"),
		_T("Открытие файла"), MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(0);			return 1;		}
		for (count = 0; in >> pt[count].x; count++) in >> pt[count].y;
		in.close(); //В переменной count сохранится размер массива точек
			hDash = CreatePen(PS_DASH, 1, 0);
			hBezier = CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
			hRect = CreateSolidBrush(RGB(255, 0, 0));
			hSel = CreateSolidBrush(RGB(255, 0, 0));	
			break;	case WM_SIZE:		sx = LOWORD(lParam);
		sy = HIWORD(lParam);		break;	case WM_RBUTTONDOWN:		point.x = LOWORD(lParam);		point.y = HIWORD(lParam);
		//Преобразование экранных координат мыши в логические
		DcInLp(point);		for (i = 0; i < count; i++)		{
		SetRect(&rt, pt[i].x - MARK, pt[i].y -MARK, pt[i].x + MARK, pt[i].y + MARK);
			if (PtInRect(&rt, point))			{ //Курсор мыши попал в точку
				index = i;hdc = GetDC(hWnd); //Захват мыши				/*POT[0]=K1[point.x]; POT[1]=K2[point.y];*/
		transform(hdc); //Переход в логические координаты
	FillRect(hdc, &rt, hSel);//Отметим прямоугольник цветом
			ReleaseDC(hWnd, hdc); SetCapture(hWnd);
				if(point.x>297&&point.y>351)
MessageBox(hWnd, POT,_T("Point"), MB_OK);
else if(point.x>232&&point.y>407)		MessageBox(hWnd, POT2,_T("Point"), MB_OK);
else if (point.x<232&&point.y<407)	MessageBox(hWnd, POT4,_T("Point"), MB_OK);
else if (point.x<120&&point.y<302)	MessageBox(hWnd, POT0,_T("Point"), MB_OK);		
		 return 0; }}	break;	case WM_RBUTTONUP:	if (capture)	{
			ReleaseCapture(); //Освобождение мыши
			capture = false;	}		break;
	case WM_MOUSEMOVE:		if (capture)		{ //Мышь захвачена
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			DcInLp(point); //Преобразование экранных координат мыши
			pt[index] = point; //в логические координаты
			InvalidateRect(hWnd, NULL, TRUE); }break;
	case WM_PAINT:		hdc = BeginPaint(hWnd, &ps);		
		hx = (max_x - min_x) / scaleX;
		hy = (max_y - min_y) / scaleY;
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, SCALE, -SCALE, NULL);
		SetViewportExtEx(hdc, sx, sy, NULL);
		SetViewportOrgEx(hdc, 0, sy, NULL);
		SetTextAlign(hdc, TA_RIGHT | TA_TOP);
		for (z = min_x, i = 0; i <= scaleX; z += hx, i++)		{
		x = int((z - min_x)*SCALE / (max_x - min_x));
		_stprintf_s(s, _T("%.1lf"), z);			TextOut(hdc, 27, 60, s, _tcslen(s));
	MoveToEx(hdc,40,30, NULL);LineTo(hdc, 40, 700);		}
MoveToEx(hdc, 0, 0, NULL); LineTo(hdc, SCALE, 0);		SetTextAlign(hdc, TA_RIGHT | TA_BOTTOM);		for (z = min_y, i = 0; i <= scaleY; z += hy, i++)		{
			y = int((z - min_y)*SCALE / (max_y - min_y) + 0.5);
			_stprintf_s(s, _T("%.1lf"), z); TextOut(hdc, 27, 60, s, _tcslen(s));
			MoveToEx(hdc, 40, 40, NULL); LineTo(hdc, 500, 40);		}
TextOut(hdc, 150, 8, nameG1, _tcslen(nameG1));TextOut(hdc, 250, 8, nameG2, _tcslen(nameG2));
TextOut(hdc, 350, 8, nameG3, _tcslen(nameG3));TextOut(hdc, 450, 8, nameG4, _tcslen(nameG4));
TextOut(hdc, 27, 150, nameG1, _tcslen(nameG1));TextOut(hdc, 27, 250, nameG2, _tcslen(nameG2));TextOut(hdc, 27, 350, nameG3, _tcslen(nameG3));
TextOut(hdc,27 , 450, nameG4, _tcslen(nameG4));TextOut(hdc,27 , 550, nameG5, _tcslen(nameG5));	MoveToEx(hdc, 0, 0, NULL); LineTo(hdc, 0, SCALE);SelectObject(hdc, hBezier);		Polyline(hdc, pt, count);		for (i = 0; i < count; i++)		{
		SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK, pt[i].x + MARK, pt[i].y + MARK);
			FillRect(hdc, &rt, hRect);		}
		EndPaint(hWnd, &ps);		break;
	case WM_DESTROY:		DeleteObject(hDash);		DeleteObject(hBezier);	DeleteObject(hRect);		DeleteObject(hSel);
	out.open("dat.txt");		for (i = 0; i<count; i++) out << pt[i].x << '\t' << pt[i].y << '\n';
		out.close();		PostQuitMessage(0);		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);	}	return 0;
}// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	UNREFERENCED_PARAMETER(lParam);	switch (message)	{
	case WM_INITDIALOG:		return (INT_PTR)TRUE;
case WM_COMMAND: if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE; }break; }return (INT_PTR)FALSE;}
