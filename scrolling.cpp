#include "stdafx.h"
#include "ГЛ2з6.h"
#include <commdlg.h>
#include <fstream>
#include <vector>
#include <string>
#include <commctrl.h>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
const DWORD MaxLength = 0x7fff;
static TCHAR name[256] = _T("");;
static OPENFILENAME file;
std::ifstream in;
std::ofstream out;
//static std::vector<std::string> v;
//std::vector<std::string>::iterator it;
//std::string st;
TBBUTTON tbb[] =
{
	{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 }
};
int y, k;
static int n, length, sx, sy, cx, iVscrollPos, iHscrollPos, COUNT, MAX_WIDTH;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY26, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY26));

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



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY26));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY26);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
    
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static TCHAR name[256] = _T("");;
	static OPENFILENAME file;
	DWORD result; int y, k;	static HANDLE hFile;
	static char text[MaxLength];
	static DWORD nCharRead;
	RECT rt; static int n, length, sx, sy, cx, iVscrollPos, iHscrollPos, COUNT, MAX_WIDTH;
	static SIZE size = { 8, 16 };
	static HWND hWndToolBar;
	static int size_Toolbar;
	static LOGFONT lf;
	static CHOOSEFONT cf;
	static HFONT hfont; TEXTMETRIC tm; static int pp = 0; static char *j;
	switch (message)
	{
	case WM_CREATE:
		file.lStructSize = sizeof(OPENFILENAME);
		file.hInstance = hInst;
		file.lpstrFilter = _T("Text\0*.txt\0Все файлы\0*.*");
		file.lpstrFile = name;
		file.nMaxFile = 256;
		file.lpstrInitialDir = _T(".\\");
		file.lpstrDefExt = _T("txt");
		hWndToolBar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | CCS_TOP, 1, 0,
			HINST_COMMCTRL, IDB_STD_SMALL_COLOR, tbb, 3, 0, 0, 0, 0, sizeof(TBBUTTON));
		cf.lStructSize = sizeof(CHOOSEFONT);
		cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
		cf.hwndOwner = hWnd;
		cf.lpLogFont = &lf;
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		k = n - (sy - size_Toolbar) / size.cy;
		if (k > 0) COUNT = k; else COUNT = iVscrollPos = 0;
		SetScrollRange(hWnd, SB_VERT, 0, COUNT, FALSE);
		SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
		k = length - sx / size.cx;
		if (k > 0) MAX_WIDTH = k; else MAX_WIDTH = iHscrollPos = 0;
		SetScrollRange(hWnd, SB_HORZ, 0, MAX_WIDTH, FALSE);
		SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);
		SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
		GetWindowRect(hWndToolBar, &rt);
		size_Toolbar = rt.bottom - rt.top;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_NEW:
			nCharRead = 0;
			n = length = 0;
			SendMessage(hWnd,WM_SIZE,0,sy<<16|sx);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_OPEN:
			file.lpstrTitle = _T("Открыть файл для чтения");
			file.Flags = OFN_HIDEREADONLY;
			if (!GetOpenFileName(&file)) return 1;
			hFile = CreateFile(name, GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			ReadFile(hFile, text, MaxLength, &nCharRead, NULL);
			CloseHandle(hFile);
			if (nCharRead == MaxLength)
			{
				MessageBox(hWnd, _T("Слишком большой файл"),
					_T("Неудачное открытие файла"), MB_YESNO | MB_ICONHAND);
				return 0;
			}
			n = length = 0; j = text;
			while (*j++)
			{
				pp++; if (*j == '\n')
				{
					n++; if (length < pp)length = pp;
					/*pp = 0;*/
				}

			}
			length += 10;
			SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_SAVE:
			file.lpstrTitle = _T("Открыть файл для записи");
			file.Flags = OFN_NOTESTFILECREATE;
			if (!GetSaveFileName(&file)) return 1;
			hFile = CreateFile(name, GENERIC_WRITE, 0, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(hFile, text, nCharRead, &result, NULL);
			CloseHandle(hFile);
			break;
		case IDM_EXIT: DestroyWindow(hWnd); break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case ID_FONT:
		if (ChooseFont(&cf))
		{
			if (hfont) DeleteObject(hfont);
			hfont = CreateFontIndirect(&lf);
			hdc = GetDC(hWnd);
			SelectObject(hdc, hfont);
			GetTextMetrics(hdc, &tm);
			size.cx = tm.tmAveCharWidth;
			size.cy = tm.tmHeight + tm.tmExternalLeading;
			ReleaseDC(hWnd, hdc);
			SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_MOUSEWHEEL:
		iVscrollPos -= (short)HIWORD(wParam) / WHEEL_DELTA;
		iVscrollPos = max(0, min(iVscrollPos, COUNT));
		SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: iVscrollPos--; break;
		case SB_LINEDOWN: iVscrollPos++; break;
		case SB_PAGEUP: iVscrollPos -= sy / size.cy; break;
		case SB_PAGEDOWN: iVscrollPos += sy / size.cy; break;
		case SB_THUMBPOSITION: iVscrollPos = HIWORD(wParam); break;
		}
		iVscrollPos = max(0, min(iVscrollPos, COUNT));
		if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
		{
			SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: iHscrollPos--; break;
		case SB_LINEDOWN: iHscrollPos++; break;
		case SB_PAGEUP: iHscrollPos -= 8; break;
		case SB_PAGEDOWN: iHscrollPos += 8; break;
		case SB_THUMBPOSITION: iHscrollPos = HIWORD(wParam); break;
		}
		iHscrollPos = max(0, min(iHscrollPos, MAX_WIDTH));
		if (iHscrollPos != GetScrollPos(hWnd, SB_HORZ))
		{
			SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hfont)
		{
			SelectObject(hdc, hfont);
			SetTextColor(hdc, cf.rgbColors);
		}
		SetRect(&rt, -iHscrollPos*size.cx,-iVscrollPos*size.cy, sx, sy);
		DrawTextA(hdc, text, nCharRead, &rt, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
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
