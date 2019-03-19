#include "stdafx.h"  // Обязательно включаемые файлы
#include "string.h" //заголовочный файл с классами для работы со строками
#include "stdlib.h" //  заголовочный файл стандартной библиотеки
#include "windows.h"//служит для вызова других файлов включений
#include "ЗАДАНИЕ 7.h"
#define MAX_LOADSTRING 100 //макрос
// Глобальные переменные:
HINSTANCE hInst;					// текущий экземпляр
HFONT newF,oldF = NULL;
TCHAR szTitle[MAX_LOADSTRING];			// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];		// имя класса главного окна
// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
// TODO: разместите код здесь.
	MSG msg;	HACCEL hAccelTable;
	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY7));
	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
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
	wcex.hIcon		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY7));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY7);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   КОММЕНТАРИИ:  В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также //создается и выводится на экран главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
int x = 60;int y = 30;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;//переменные для вывода в окно при обработке сообщения WM_PAINT
	TCHAR *str= _T("Вывод текста"); //символьный тип
	static RECT rc;
	static int sx, sy;
	GetClientRect(hWnd, &rc);//определение фактического размера клиентской области
	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);//ширина окна
		sy = HIWORD(lParam);//высота окна
		break;
	case WM_LBUTTONDOWN:
		if (x < 72 || y < 30) { x++; y++; }//увеличиваем текст
		InvalidateRect(hWnd, NULL, TRUE);//объявление окна недействительным для перерисовки фона окна
	break;
	case WM_RBUTTONDOWN:
		if (x < 75&&x>40 || y < 35&&y>12){ x--; y--; }//уменьшаем текст
		InvalidateRect(hWnd, NULL, TRUE);
	break;
	case WM_PAINT:
	hdc = BeginPaint(hWnd, &ps);//получаем hdc
newF = CreateFont(x, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));//создание шрифта высотой x,шириной 0,угол наклона строки текста 0,угол наклона каждого символа 0,жирность 400, не курсив,не подчеркнутый шрифт,не перечеркнутый,множество символов default_charset,точность отображения шрифта, определяет как будут отсекаться символы, качество шрифта default_quality ,тип и семейство шрифтов,указатель на строку с именем шрифта
	oldF =(HFONT)SelectObject(hdc, newF);//переменной  oldF присваиваем newF
	TextOut(hdc, 0, 0, str, _tcslen(str));//выв текста 
	SelectObject(hdc, oldF);//выбор шрифта
	DeleteObject(newF);//удаляем  старый шрифт
	EndPaint(hWnd, &ps);//освобождение контекста устройства
	break;
	case WM_DESTROY://завершение прог-мы пользователем
		PostQuitMessage(0);break;//завершает приложение и передает ос код возврата 0
	default:	return DefWindowProc(hWnd, message, wParam, lParam);//если не выполнилась ни одна из альтернатив , то возврат ф-и обработки остальных сообщений по умолчанию
	}
	return 0;
}
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
