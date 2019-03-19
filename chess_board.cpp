#include "stdafx.h"
#include "Шахматная доска.h"
#include "assert.h"
#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;		// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];		// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];	// имя класса главного окна
int i, j; POINT pt;  int R = 500, L = 500;
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
	MSG msg;
	HACCEL hAccelTable;
	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{	return FALSE;	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));
	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0)){
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{	TranslateMessage(&msg);
			DispatchMessage(&msg);}}	return (int) msg.wParam;}
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	return RegisterClassEx(&wcex);}
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   КОММЕНТАРИИ://        В данной функции дескриптор экземпляра сохраняется в //глобальной переменной, а также  создается и выводится на экран главное окно //программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0,  CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {      return FALSE;   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);   return TRUE;}
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	PAINTSTRUCT ps;//вывод в окно при обработке сообщения WM_PAINT
	HDC hdc;//дескриптор контекста устройства
	static int wmId, wmEvent; static int xpos, ypos;
	static int a, b/*, x, y*/;   // ширина одного квадрата
	HBRUSH hBrWhite, hOldBrWhite, hBr, hOldBr;
	HPEN hPen, hOldPen;
	TCHAR L_str[9] = _T("87654321");
	TCHAR N_str[9] = _T("ABCDEFGH");
	static TCHAR Yach[1]; static int vy, vx,kiki,qop;	
	switch (message)
	{	case WM_SIZE:
//размеры окна
		wmId = LOWORD(lParam);
		wmEvent = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);//получение контекста уср-ва
		//размеры доски
		a = (wmId / 9) ;
		b = (wmEvent / 9) ;
		a = b;
		hPen = CreatePen(PS_NULL, 0, 0);//создание прозрачного пера
		hOldPen = (HPEN)SelectObject(hdc, hPen);//установка прозрачного пера
		hBr = CreateSolidBrush(RGB(209, 139, 71));//создание сплошной кисти
		hOldBr = (HBRUSH)SelectObject(hdc, hBr);// установка сплошной кисти
		hBrWhite = CreateSolidBrush(RGB(255, 206, 158));//создание сплошной кисти
		hOldBrWhite = (HBRUSH)SelectObject(hdc, hBr);// установка кисти hBr
		for (i = 0; i<8; i++)//циклы для строк и столбцов доски
		for (j = 0; j<8; j++)
		{//если и строки,и столбцы-четны или нечетны устанавливаем кисть hBr
			if ((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
			{   SelectObject(hdc, hBr);	}
//иначе – кисть hBrWhite
		else SelectObject(hdc, hBrWhite);
		Rectangle(hdc, (i*a)+20, (j*b)+20,( i*a + a)+20, (j*b + b)+20);//создаем прям-ки
		TextOut(hdc,2, (i*a)+45 , &L_str[i], 1);//текст -цифры
		TextOut(hdc,(j*b)+45,2 , &N_str[j], 1); }//текст буквы
		//удаляем кисти и перо
		DeleteObject(SelectObject(hdc, hOldBr));
		DeleteObject(SelectObject(hdc, hOldBrWhite));
		DeleteObject(SelectObject(hdc, hOldPen));	
		EndPaint(hWnd, &ps);return 0;//завершаем обработчик сообщения 
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);//получаем контекст устр-ва
		xpos = LOWORD(lParam);//текущие координаты курсора
		ypos = HIWORD(lParam);
		//позиция курсора относительно доски
		vx = xpos / a;
		vy = ypos  / b;
		for (i = 0; i<8; i++)// циклы для строк и столбцов доски
			for ( j = 0; j < 8; j++)	{};
//если координаты курсора за границами доски, то выводи сообщение об ошибке
		if (xpos>(i*a) || ypos>(j*b) ||xpos>(i*a + a) || ypos>(j*b + b)) MessageBox(hWnd,_T("Error"), _T("Ячейка"), 0);
//иначе выводим название ячейки
		else {	Yach[0] = N_str[vx];
			Yach[1] = L_str[vy];
			MessageBox(hWnd, Yach, _T("Ячейка"), 0);}
		ReleaseDC(hWnd, hdc);//освобождаем контекст устройства	
		break;
	case WM_DESTROY:
		PostQuitMessage(0);//завершение приложения и передают ос код возврата 0
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);//если не //выполнилась ни одна из альтернатив, то возврат ф-и обработки остальных сообщений по //умолчанию
	}	return 0;}
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{case WM_INITDIALOG:	return (INT_PTR)TRUE;
	case WM_COMMAND:	if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)	{	EndDialog(hDlg, LOWORD(wParam)); return (INT_PTR)TRUE;}
		break;}	return (INT_PTR)FALSE;} 
