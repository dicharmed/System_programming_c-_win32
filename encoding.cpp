#include "stdafx.h"
#include "2.3.h"
#include <commdlg.h>// подключение функций файлового диалога
#include <fstream>// содержит определение классов ifstream,ofstream
#include <vector>
#include <string>
#include <commctrl.h>
#define MAX_LOADSTRING 100
// Глобальные переменные:
TBBUTTON tbb[] ={
{STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0,0,0},
{STD_FILEOPEN, ID_FILE_OPEN,TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0,0,0},
{STD_FILESAVE, ID_FILE_SAVE,TBSTATE_ENABLED, TBSTYLE_BUTTON,0,0,0,0}
};HINSTANCE hInst;							// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];				// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(HINSTANCE hInstance,
 HINSTANCE hPrevInstance, LPTSTR    lpCmdLine, int nCmdShow){ UNREFERENCED_PARAMETER(hPrevInstance);UNREFERENCED_PARAMETER(lpCmdLine);// TODO: разместите код здесь.
MSG msg;	HACCEL hAccelTable;// Инициализация глобальных строк
LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
LoadString(hInstance, IDC_MY23, szWindowClass, MAX_LOADSTRING);
MyRegisterClass(hInstance);// Выполнить инициализацию приложения:
if (!InitInstance (hInstance, nCmdShow))	{return FALSE;}
hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY23));
// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){TranslateMessage(&msg);
			DispatchMessage(&msg);}}	return (int) msg.wParam;}
//  ФУНКЦИЯ: MyRegisterClass()
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//  КОММЕНТАРИИ:
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный //код   был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
ATOM MyRegisterClass(HINSTANCE hInstance)
{	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY23));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY23);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	return RegisterClassEx(&wcex);}
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//   КОММЕНТАРИИ:  В данной функции дескриптор экземпляра сохраняется в глобальной //переменной, а также     создается и выводится на экран главное окно программы.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{   HWND hWnd;   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)   {      return FALSE;   }
   ShowWindow(hWnd, nCmdShow);   UpdateWindow(hWnd);   return TRUE;}
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ PAINTSTRUCT ps;// переменная для вывода в окне при обработке сообщения
HDC hdc;// дескриптор контекста устройства
static TCHAR name[256] = _T("");//статическая переменная для строки
static OPENFILENAME file;// структура для организации диалога выбора файла
std::ifstream in;// определение потока ввода
std::ofstream out;// определение потока вывода
static std::vector<std::string> v;// создается временный "пустой" вектор для переменных типа  /*string */std::vector<std::string>::iterator it;// создается временный "пустой" вектор для //переменных типа  string класса iterator
std::string st;// создали переменную типу string
int y, k;static int n,length,sx,sy,cx,iVscrollPos,iHscrollPos,COUNT,MAX_WIDTH;
static SIZE size = {8, 16 };//Ширина и высота символа
static HWND hWndToolBar;     static int size_Toolbar;  RECT rt;  static char name2[256]=" ";
static LOGFONT lf;  static CHOOSEFONT cf;  static HFONT hfont;  TEXTMETRIC tm;
switch (message)
{ case WM_CREATE:
file.lStructSize = sizeof(OPENFILENAME);//Размер структуры
file.hInstance = hInst;//Дескриптор приложения
file.lpstrFilter = _T("Text .txt\0 *.txt\0Все файлы\0 *.*");//Фильтр из пар строк
file.lpstrFile = name;//Указатель на строку с именем файла
file.nMaxFile = 256;// Размер буфера для имени файла
file.lpstrInitialDir = _T(".\\");//Указатель на строку начального каталога
file.lpstrDefExt = _T("txt");// Указатель на строку с расширением имени файла по умолчанию
hWndToolBar = CreateToolbarEx(hWnd,WS_CHILD|WS_VISIBLE|CCS_TOP,1,0,
HINST_COMMCTRL,IDB_STD_SMALL_COLOR,tbb,3,0,0,0,0,sizeof(TBBUTTON));//окно дочернее,отображается автоматически,идентификатор окна панели инструмен-в
cf.lStructSize = sizeof(CHOOSEFONT);//содержит размер структуры CHOOSEFONT
cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;//определяют флаги 
cf.hwndOwner = hWnd;//определяют дескриптор родительского окна
cf.lpLogFont = &lf;break;//хранит указатель на переменную типа LOGFONT
case WM_SIZE:
sx = LOWORD(lParam);// ширина окна
sy = HIWORD(lParam);// высота окна
k = n - (sy - size_Toolbar)/size.cy;// Диапазон вертикальной прокрутки
if (k > 0) COUNT = k; else COUNT = iVscrollPos = 0;//определение диапазона вертикального скроллинга
SetScrollRange(hWnd, SB_VERT, 0, COUNT, FALSE);// установка диапазона 
SetScrollPos (hWnd, SB_VERT, iVscrollPos, TRUE);// позиция скроллинга
k = length - sx/size.cx;
if (k > 0) MAX_WIDTH = k; else MAX_WIDTH = iHscrollPos = 0;
SetScrollRange(hWnd, SB_HORZ, 0, MAX_WIDTH, FALSE);// установка диапазона
SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);// позиция скроллинга
SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);//корректировка размера панели инструментов
GetWindowRect(hWndToolBar, &rt);//координаты окна относительно рабочего стола
size_Toolbar = rt.bottom - rt.top;break;
case WM_MOUSEWHEEL: //Прокрутка колесика мыши
iVscrollPos -= (short)HIWORD(wParam)/WHEEL_DELTA; // в старшее слово возвращ кол-во шагов колесика, умножен-е на константу WHEEL_DELTA(120).Делим это значение на константу и получаем количество шагов
iVscrollPos = max(0, min(iVscrollPos, COUNT));// присваиваем значение, чтобы она не вышла за границы отрезка [0, COUNT]
SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);//устанав нов позиции чтобы не выйти за границы скрол-га
InvalidateRect(hWnd, NULL, TRUE);//перерисовка окнаbreak;
case WM_VSCROLL :
switch(LOWORD(wParam)){ case SB_LINEUP : iVscrollPos--; break;// движок на строку вверх
case SB_LINEDOWN : iVscrollPos++; break;// движок на строку вниз 
case SB_PAGEUP : iVscrollPos -= sy/size.cy; break;// движок на страницу вверх
case SB_PAGEDOWN : iVscrollPos += sy/size.cy; break;// движок на страницу вниз
case SB_THUMBPOSITION : iVscrollPos = HIWORD(wParam); break;// обработка сообщения о том, что позиция движка была изменена}
iVscrollPos = max(0, min(iVscrollPos, COUNT)); if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
{ SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE); InvalidateRect(hWnd, NULL, TRUE);
}break;
case WM_HSCROLL :
switch(LOWORD(wParam)){ case SB_LINEUP : iHscrollPos--; break; case SB_LINEDOWN : iHscrollPos++; break; case SB_PAGEUP : iHscrollPos -= 8; break; case SB_PAGEDOWN : iHscrollPos += 8; break; case SB_THUMBPOSITION : iHscrollPos = HIWORD(wParam); break;}
iHscrollPos = max(0, min(iHscrollPos, MAX_WIDTH)); if (iHscrollPos != GetScrollPos(hWnd, SB_HORZ)){ SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE); InvalidateRect(hWnd, NULL, TRUE); }break;
case WM_COMMAND:
switch (LOWORD(wParam))
{//обработка сообщения для выбора пункта меню, когда элемент управления посылает уведомительное сообщение родительскому окну и происходит нажатие клавиши акселератора
case ID_FILE_NEW :
if (!v.empty()) std::vector<std::string>().swap(v); //очищаем вектор
n = length = 0; //обнуляем переменные описывающие кол-во строк файла и макс длину строки
SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx); //передаем сообщение окну,в старшее слово помещаем высоту окна с помощью логического сдвига sy << 16 | sx,а в младшее слово 0,т.к. не использ-ся
InvalidateRect(hWnd,NULL,TRUE);break;
case ID_FILE_OPEN :
file.lpstrTitle = _T("Открыть файл для чтения");// выв заголовка диалог окна
file.Flags = OFN_HIDEREADONLY;//переключатель только для чтения не выв 
if (!GetOpenFileName(&file)) return 1;// ;//если работа функц успеш,то в name получ полное имя файла и открываем методом open()
in.open(name);
while (getline(in,st)) //читаем построчно файл из потока in чит строку и перемещ в переменную st, пока файл весь не будет прочитан
{if (length < st.length()) length = st.length();//добав строку,по заверш чтения файла в length будет размер строки макс длины	
v.push_back(st);// стр в конец контейн
}in.close();//закрыв входной поток
n = v.size();//получ кол строкфайла как размер контейнера
SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
InvalidateRect(hWnd,NULL,1);break;
case ID_FILE_SAVE :
file.lpstrTitle = _T("Открыть файл для записи");
file.Flags = OFN_NOTESTFILECREATE;
if (!GetSaveFileName(&file)) return 1;
out.open(name);
for (it = v.begin(); it != v.end(); ++it) out << *it << '\n';//цикл записи в файл содержим контейн от нач до кон контейнр по итератору it,добав в кон кажд стр символ перевода стр
out.close();break;
case ID_DOS:
file.lpstrTitle = _T("Открыть файл для чтения");
CharToOem(name,name2);//преобразует строку в определенный OEM набор символов
file.Flags = OFN_HIDEREADONLY;
if (!GetOpenFileName(&file)) return 1;
in.open(name);
while (getline(in, st))
{if (length < st.length()) length = st.length();
v.push_back(st);
}in.close(); n = v.size();
SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx); InvalidateRect(hWnd, NULL, TRUE); break;
case ID_WINDOWS:
file.lpstrTitle = _T("Открыть файл для чтения");
OemToChar(name2,name);//преобразует строку с набора символов или в ANSI 
file.Flags = OFN_HIDEREADONLY;
if (!GetOpenFileName(&file)) return 1; in.open(name);
while (getline(in, st))
{if (length < st.length()) length = st.length();
v.push_back(st);}in.close(); n = v.size();
SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
InvalidateRect(hWnd, NULL, TRUE);break;
case ID_FONT :
if(ChooseFont(&cf))//функция диалога
{if (hfont) DeleteObject(hfont);
hfont = CreateFontIndirect(&lf);//создание шрифта
hdc = GetDC(hWnd);//получаем контекст устройства
SelectObject(hdc, hfont);//устанавливаем шрифт
GetTextMetrics(hdc, &tm);//определяем метрику текста
size.cx = tm.tmAveCharWidth;//получаем среднюю ширину символа
size.cy = tm.tmHeight + tm.tmExternalLeading;//высота строки
ReleaseDC(hWnd, hdc);//освобождаем контекст
SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);//переопределяем параметры скроллинга
InvalidateRect(hWnd, NULL, TRUE); }break;
case IDM_EXIT: DestroyWindow(hWnd); break;
default: return DefWindowProc(hWnd, message, wParam, lParam); }break;
case WM_PAINT:
hdc = BeginPaint(hWnd, &ps);// получение контекста уср-ва
if (hfont){ SelectObject(hdc, hfont);SetTextColor(hdc, cf.rgbColors);}for (y = size_Toolbar, it = v.begin() + iVscrollPos; it != v.end() && y < sy; ++it, y += size.cy)// Переменная y, как и ранее, //используется для вычисления y-координаты выводимой строки текста, а итератор it, которым //мы выбираем строки текста из контейнера,сместим на iVscrollPos позиций. Цикл завершится, //если будет достигнут конец контейнера или у-координата строки выйдет за пределы окна.
if (iHscrollPos < it->length())// вывести строку текста с учетом обработки табуляций. Выводим //строки текста со смещением iHscrollPos, размер строки уменьшается на эту же величину: it-//>length()-iHscrollPos
TabbedTextOutA(hdc, 0, y, it->data()+iHscrollPos,it->length()-iHscrollPos, 0, NULL, 0); EndPaint(hWnd, &ps);break;
case WM_DESTROY: if (hfont) DeleteObject(hfont); PostQuitMessage(0);// завершение /*приложения и передают ос код возврата 0*/ break; default: return DefWindowProc(hWnd, message, wParam, lParam);//если не выполнилась ни одна из альтернатив, то возврат ф-и //обработки остальных сообщений по умолчанию}return 0;}// Обработчик сообщений для окна //"О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {UNREFERENCED_PARAMETER(lParam);	switch (message)
{case WM_INITDIALOG:	return (INT_PTR)TRUE;
case WM_COMMAND:if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
{EndDialog(hDlg, LOWORD(wParam));	return (INT_PTR)TRUE;	}break;}return (INT_PTR)FALSE;}
