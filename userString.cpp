//13.2.1 Листинг DLL-библиотеки
// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "string.h"

extern "C" __declspec(dllexport) int StrLen1(const char *, int);
extern "C" __declspec(dllexport) void StrCpy1(char *, size_t, const char *);
extern "C" __declspec(dllexport) void StrCat1(char *, size_t, const char *);
extern "C" __declspec(dllexport) void StrRev1(char *);

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call,LPVOID lpReserved )
{	
	return TRUE;
}
int  StrLen1(const char * g, int len)
{
	len = strlen(g);
	return len;
}
void  StrCpy1(char * g, size_t t, const char * n)
{
	strcpy_s(g, t, n);
}
void StrCat1(char * g, size_t t, const char * n)
{
	strcat_s(g, t, n);
}
void StrRev1(char *g)
{
	_strrev(g);
}
//13.2.2 Листинг демонстрационной программы
// demo666.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "demo666.h"
#include "string.h"
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

extern "C" __declspec(dllimport) int StrLen1(const char *, int);
extern "C" __declspec(dllexport) void StrCpy1(char *, size_t, const char *);
extern "C" __declspec(dllexport) void StrCat1(char *, size_t, const char *);
extern "C" __declspec(dllexport) void StrRev1(char *);
….
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	char ss[]="you";
	char ss1[]="love";
	int l=0,k;
	size_t t=20;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		//k-число симоволов в строке
		k=StrLen1(ss,l);
		//копируем строку
		StrCpy1(ss, t, ss1);
		//Добавляет копию строки к другой
		StrCat1(ss, t, ss1);
		//Переворачиваем строку
		StrRev1(ss);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
