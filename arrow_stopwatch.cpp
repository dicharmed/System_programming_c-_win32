#include "stdafx.h"
#include "5454.h"
# include "windows.h"
#include "windowsx.h"
#include "stdio.h"
#include "tchar.h"
#define _USE_MATH_DEFINES
#include "cmath"
#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;			// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];		// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];// имя класса главного окна
int nFunsterStil;int iwinX = 400, iwinY = 40, iwinXC = iwinX - 150, iwinYC = iwinY;
int iDia = 500, iHeight = iDia + 34, iWidth = iDia + 8, iRad = iDia / 2,sizFont = int(iDia / 6.7);
int iKorp = 8, iMetM = 5, iMetH = 10;int iRadMet = iRad - iKorp, iRadCFR = iRadMet - sizFont / 4 - iMetH;int penwKorp = 2, penwCfrbl = 2, penwmM = 2, penwmH = 3;COLORREF colKorp = RGB(0, 0, 0), colCfrbl = RGB(255, 255, 255),colStrMH = colKorp, colStrS = RGB(255, 0, 0), colmM = colKorp,colmH = colKorp, colCfr = colKorp;HWND hwnd; HDC hdcMem0, hdcMem, hdc;HBITMAP hBitmap, hOldBitmap; BITMAP bm; PAINTSTRUCT ps;HBRUSH br; HPEN pen; HFONT hFontO, hOldFont;MSG mMsg;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void CYFERBL(HWND);//ФУНКЦИЯ РИСОВАНИЯ ЦИФЕРБЛАТА
void STRELKA(COLORREF, int, int, float);//ФУНКЦИЯ РИСОВАНИЯ СТРЕЛКИ
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
 LPTSTR    lpCmdLine,        int     CmdShow){UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: разместите код здесь.
	MSG msg;	HACCEL hAccelTable;
// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY5454, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{	return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY5454));
	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{	TranslateMessage(&msg);
			DispatchMessage(&msg);}}return (int) msg.wParam;}
//  ФУНКЦИЯ: MyRegisterClass()
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//  КОММЕНТАРИИ:
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный //код  был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY5454));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY5454);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);}
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   КОММЕНТАРИИ:
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {      return FALSE;   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);   return TRUE;}
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
static long double k = ((2 * M_PI) / 60) * 60;//статическая переменная для инициализации угла
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int t; //статическая переменная для таймера
	static double cx, cy;
	switch (message)
{
	case  WM_CREATE:
		SetWindowPos(hwnd, NULL/* дескриптор порядка размещения*/, iwinX, iwinY, iWidth, iHeight, 	SWP_NOSIZE //Сохраняет текущий размер);//функция устанавливающая //позицию окна
		hdc = GetDC(hwnd); //получаем контекст устройства
		hdcMem = CreateCompatibleDC(hdc);//Создает контекст устpойства памяти
		hdcMem0 = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, iDia, iDia);//создает точечный рисунок, //совместимый с устройством
		GetObject(hBitmap, sizeof(BITMAP), &bm);// получаем информацию об объекте
		hOldBitmap = SelectBitmap(hdcMem0, hBitmap);
		CYFERBL(hwnd);  // ВЫЗОВ ФУНКЦИИ ЦИФЕРБЛАТА
		break;
	case WM_PAINT:
		float strUgolS, strUgolM, strUgolH, strUgolMS;//объявление переменных угла
		hdcMem = BeginPaint(hwnd, &ps);//получение контекста устройства
		BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem0, 0, 0, SRCCOPY);//Отрисовка изображения
		//ВЫЗОВ СТРЕЛКИ 
		STRELKA(colStrS, iRadMet - iMetH - iMetM, 1, strUgolS);//СЕКУНДНАЯ СТРЕЛКА
		//ЦЕНТРАЛЬНАЯ ТОЧКА
pen = CreatePen(PS_SOLID, 1, colStrMH);//создание пера
br = CreateSolidBrush(colStrMH);//создание сплошной кисти
SelectBrush(hdcMem, br);//установка кисти
Ellipse(hdcMem, iRad - 3, iRad - 3, iRad + 3, iRad + 3);//рисование эллипса
DeleteObject(br);//удаляем кисть
DeleteObject(pen);//удаляем перо
BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);//Отрисовка изображения
		ReleaseDC(hwnd, hdc);//освобождаем контекст устройства
		ReleaseDC(hwnd, hdcMem);
		ReleaseDC(hwnd, hdcMem0);
		DeleteObject(hBitmap);//удаляем объект
		EndPaint(hwnd, &ps); break;//завершаем обработчик сообщения
	case WM_CHAR:KillTimer(hwnd, 1); break;//уничтожаем таймер
	case WM_LBUTTONDOWN:SetTimer(hwnd, 1, 1000, NULL);  break;//запускаем таймер при //нажатии на левую кнопку мыши
	case WM_RBUTTONDOWN:
		k = ((2 * M_PI) / 60) * 61;//сброс таймера при нажатии на правую кнопку мыши
		t = 0;	KillTimer(hwnd, 1); break;//уничтожаем таймер
	case WM_TIMER:
		t++;	k = k - ((2 * M_PI) / 60);
		InvalidateRect(hwnd, NULL, TRUE); break;//перерисовка окна
	case WM_DESTROY:
		KillTimer(hwnd, 1);//уничтожаем таймер
		DeleteBitmap(hBitmap);//удаляем объект
		SelectBitmap(hdcMem, hOldBitmap);
		DeleteDC(hdcMem);//освобождаем контекст устройства
		DeleteDC(hdcMem0);
		PostQuitMessage(0);//завершение приложения и передают ос код возврата 0 
		break;
	default: return DefWindowProc(hwnd, message, wParam, lParam);////если не выполнилась ни одна из альтернатив , то возврат ф-и обработки остальных сообщений по умолчанию}}
void CYFERBL(HWND hwnd)
{
	int ii, jj, iX, iY, iXc, iYc, t = 0;
	float fUgol, fUgolc;   _TCHAR str[12];
	// РИСОВАНИЕ ЦИФЕРБЛАТА
	BeginPaint(hwnd, &ps);
	// ЗАЛИВКА КОРПУСА
	br = CreateSolidBrush(RGB(255, 255, 255));//создание сплошной кисти
	SelectObject(hdcMem0, br);//установка кисти
	Rectangle(hdcMem0, 0, 0, iDia, iDia);//создание прямоугольника
	DeleteObject(br);//удаление кисти
	pen = CreatePen(PS_SOLID, 1, colKorp);//создание пера
	SelectObject(hdcMem0, pen);//установка пера
	br = CreateSolidBrush(colKorp);//создание сплошной кисти
	SelectObject(hdcMem0, br);//установка кисти
	Ellipse(hdcMem0, 0, 0, iDia, iDia);//создание эллипса
	DeleteObject(br); DeleteObject(pen);//удаление кисти и пера
	// ЗАЛИВКА ЦИФЕРБЛАТА
	br = CreateSolidBrush(colCfrbl);//создание сплошной кисти
	SelectObject(hdcMem0, br);//установка кисти
	Ellipse(hdcMem0, iKorp, iKorp, iDia - iKorp, iDia - iKorp);//создание эллипса
	DeleteObject(br);//удаление кисти
	//МЕТКИ  секунд
	pen = CreatePen(PS_SOLID, penwmM, colmM);//создание пера
	SelectObject(hdcMem0, pen);//установка пера
	for (ii = 1; ii <= 60; ii++)//цикл для меток 
	{
		fUgol = -0.017453292 * 6 * ii;
		iX = iRad - int(iRadMet*sin(fUgol));
		iY = iRad - int(iRadMet*cos(fUgol));
		MoveToEx(hdcMem0, iX, iY, NULL);//установка текущей позиции
		iX = iRad - int((iRadMet - iMetM)*sin(fUgol));
		iY = iRad - int((iRadMet - iMetM)*cos(fUgol));
		LineTo(hdcMem0, iX, iY);//рисование линии
	}
	DeleteObject(pen);//удаление пера
	//МЕТКИ
	pen = CreatePen(PS_SOLID, penwmH, colmH);//создание пера
	SelectObject(hdcMem0, pen);//установка пера
	for (ii = 1; ii <= 12; ii++)//цикл для меток
	{
		fUgol = -0.017453292 * 30 * ii;
		iX = iRad - int(iRadMet*sin(fUgol));
		iY = iRad - int(iRadMet*cos(fUgol));
		MoveToEx(hdcMem0, iX, iY, NULL);
		iX = iRad - int((iRadMet - iMetH)*sin(fUgol));
		iY = iRad - int((iRadMet - iMetH)*cos(fUgol));
		LineTo(hdcMem0, iX, iY);
	}
	DeleteObject(pen);//удаление пера
	//ШРИФТ ЦИФР
	SetBkMode(hdcMem0, TRANSPARENT);//определение цвета фона
	SetTextColor(hdcMem0, colCfr);//определение цвета текста
	//ВЫВОД ЦИФР 
	for (jj = 1; jj <= 12; jj++)//цикл для цифр на циферблате
	{
		fUgolc = -0.017453292 * 30 * jj;
		iXc = iRad - int(iRadCFR*sin(fUgolc)) - sizFont / 8;
		iYc = iRad - int(iRadCFR*cos(fUgolc)) - sizFont / 8;

		hFontO = CreateFont(30, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));//определение шрифта
		hOldFont = (HFONT)SelectObject(hdcMem0, hFontO);//установка текущего шрифта
		_stprintf(str, _T("%d"), jj, 0);//Выводит отформатированную строку 
		TextOut(hdcMem0, iXc, iYc, str, _tcsclen(str));//текст цифр
		SelectObject(hdcMem0, hOldFont);//установка шрифта
		DeleteObject(hFontO);//удаление шрифта
	}
	EndPaint(hwnd, &ps);//завершаем обработчик сообщения
}
void STRELKA(COLORREF colStr, int radSTR, int shirSTR, float strUgol)
{
int okrR = 3 * shirSTR;
int penwSTR = 2;
int strX1 = iRad + int(0.3*radSTR * sin(k) - shirSTR*cos(k));
int strY1 = iRad + int(0.3*radSTR * cos(k) + shirSTR*sin(k));
int strX2 = iRad + int(0.3*radSTR * sin(k) + shirSTR*cos(k));
int strY2 = iRad + int(0.3*radSTR * cos(k) - shirSTR*sin(k));
int strX = iRad - int(radSTR * sin(k));
int strY = iRad - int(radSTR * cos(k));
POINT pt[3] = { { strX1, strY1 }, { strX2, strY2 }, { strX, strY } };//массив точек
pen = CreatePen(PS_SOLID, penwSTR, colStr);//создание пера
	SelectObject(hdcMem, pen);//установка пера
	br = CreateSolidBrush(colStr);//создание кисти
	SelectObject(hdcMem, br);//установка кисти
	Polygon(hdcMem, pt, 3);//создание многоугольника
	Ellipse(hdcMem, iRad - okrR, iRad - okrR, iRad + okrR, iRad + okrR);//создание эллипса
	DeleteObject(br);//удаление кисти
	DeleteObject(pen);//удаление пера
}//STRELKA
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
		EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;}
		break;}
	return (INT_PTR)FALSE;}
