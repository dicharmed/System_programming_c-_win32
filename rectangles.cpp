#include "stdafx.h"
#include "задание 5.h"
#include <time.h>
#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;	// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];// имя класса главного окна
int R=800, L=600;//размеры окна
// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,  _In_opt_ HINSTANCE hPrevInstance,         _In_ LPTSTR    lpCmdLine,                     _In_ int       nCmdShow)
{	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: разместите код здесь.
	MSG msg;	HACCEL hAccelTable;
	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{		return FALSE;	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE (IDC_MY5));
	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{	TranslateMessage(&msg);
			DispatchMessage(&msg);		}
	}
	return (int) msg.wParam;
}
//  ФУНКЦИЯ: MyRegisterClass()
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance,  MAKEINTRESOURCE(IDI_MY5));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY5);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance,  MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   КОММЕНТАРИИ:
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а //также    создается и выводится на экран главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{   HWND hWnd;
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,0, 0,R,L, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {      return FALSE;   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.

int x, y, wx, wy;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static int t;
	int a, b, c;
	RECT r;
	static int mx, my;
	switch (message)
	{
	case WM_SIZE://физ размеры окна
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		break;
	case WM_CREATE://создаем таймер с периодичностью 0,1 сек
		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_TIMER://увеличиваем t на 1 сек и объявляем недействительным окно
		t++;
		InvalidateRect(hWnd, NULL, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);//получение контекста устр-ва
		HBRUSH br, obr;
		srand(time(NULL));//генератор случ чисел
		//случайный отбор для цвета прямоугольников		
		a = rand() % 255;
		b = rand() % 255;
		c = rand() % 255;
		//создаем сплошную кисть для заполнения прямоуг-в и присваиваем кисти br
		br = CreateSolidBrush(RGB(a, b, c));
		//кисти obr присваиваем кисть br и устанавливаем текущей
		obr = (HBRUSH)SelectObject(hdc, br);
		//случ отбор для координат прям-в
		x = rand() % R;
		y = rand() % L;
		wx = rand() % (R/4); 
		wy = rand() % (L /4);
		//созд прямоуг-ки
		Rectangle(hdc, x, y, (x + wx), (y + wy));
		//установки текущей кисти obr
		SelectObject(hdc, obr);
		//удаляем кисть br
		DeleteObject(br);
		//случ цвет для контура прямоуг
		a = rand() % 255;
		b = rand() % 255;
		c = rand() % 255;
		br = CreateSolidBrush(RGB(a, b, c));
		RECT rc;
		//логические координаты верхн- левого и правого низк-углов прямоуг-ка
		rc.left = x;
		rc.right = x + wx;
		rc.top = y;
		rc.bottom = y + wy;
		FrameRect(hdc, &rc, br);//рисование контура прямоуг-ка кистью br
		obr = (HBRUSH)SelectObject(hdc, br);
		SelectObject(hdc, obr);
		DeleteObject(br);
		EndPaint(hWnd, &ps);//освобождение контекста устройства
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);//уничтожаем таймер
		PostQuitMessage(0);//завершает приложение и передает ос код возврата 0
		break;
	default: return DefWindowProc(hWnd, message, wParam,  lParam);//если не выполнилась ни одна из альтернатив , то возврат ф-и обработки остальных //сообщений по умолчанию
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM  
lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) ==  
IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
