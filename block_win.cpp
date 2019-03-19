// 1222.cpp : defines the entry point for the application.
#include "stdafx.h"
#include "1222.h"
#include <commdlg.h>// подключение функций файлового диалога
#include <fstream>// содержит определение классов ifstream, ofstream
#include <vector>
#include <string>
#include <commctrl.h>
#define max_loadstring 100
// global variables:
static std::vector<std::string> v; // создается временный "пустой" вектор для переменных типа  string
std::vector<std::string>::iterator it;// создается временный "пустой" вектор для переменных типа  string класса iterator
tchar winclassgraphname[max_loadstring] = _t("childclass");//ввели имя нового класса окна
tbbutton tbb[] ={{ std_filenew, id_file_new, tbstate_enabled, tbstyle_button, 0, 0, 0, 0 },{ std_fileopen, id_file_open, tbstate_enabled, tbstyle_button, 0, 0, 0, 0 },{ std_filesave, id_file_save, tbstate_enabled, tbstyle_button, 0, 0, 0, 0 }, };
hinstance hinst;							// текущий экземпляр
tchar sztitle[max_loadstring];				// текст строки заголовка
tchar szwindowclass[max_loadstring];			// имя класса главного окна
// forward declarations of functions included in this code module:
atom				myregisterclass(hinstance hinstance);
bool				initinstance(hinstance, int);
lresult callback    wndgraph(hwnd, uint, wparam, lparam);// описали прототип оконной функции построения графика
lresult callback	wndproc(hwnd, uint, wparam, lparam);
atom                registergraphclass();
int_ptr callback	about(hwnd, uint, wparam, lparam);
int apientry _twinmain(_in_ hinstance hinstance,
                     _in_opt_ hinstance hprevinstance,
                     _in_ lptstr    lpcmdline,
                     _in_ int       ncmdshow){
	unreferenced_parameter(hprevinstance);
	unreferenced_parameter(lpcmdline);// todo: place code here.msg msg;
	haccel hacceltable;// initialize global strings
	loadstring(hinstance, ids_app_title, sztitle, max_loadstring);
	loadstring(hinstance, idc_my1222, szwindowclass, max_loadstring);
	myregisterclass(hinstance);// perform application initialization:
	if (!initinstance (hinstance, ncmdshow)){
	return false;}
	hacceltable = loadaccelerators(hinstance, makeintresource(idc_my1222));
	// main message loop:
	while (getmessage(&msg, null, 0, 0))
	{if (!translateaccelerator(msg.hwnd, hacceltable, &msg)){
			translatemessage(&msg);
			dispatchmessage(&msg);}}return (int) msg.wparam;}
//  function: myregisterclass()
//  purpose: registers the window class.
atom myregisterclass(hinstance hinstance){
	wndclassex wcex;
	wcex.cbsize = sizeof(wndclassex);
	wcex.style			= cs_hredraw | cs_vredraw;
	wcex.lpfnwndproc	= wndproc;
	wcex.cbclsextra		= 0;
	wcex.cbwndextra		= 0;
	wcex.hinstance		= hinstance;
	wcex.hicon			= loadicon(hinstance, makeintresource(idi_my1222));
	wcex.hcursor		= loadcursor(null, idc_arrow);
	wcex.hbrbackground	= (hbrush)(color_window+1);
	wcex.lpszmenuname	= makeintresource(idc_my1222);
	wcex.lpszclassname	= szwindowclass;
	wcex.hiconsm		= loadicon(wcex.hinstance, makeintresource(idi_small));
	return registerclassex(&wcex);}
atom registergraphclass(){
	wndclassex wcgraph = { 0 };
	wcgraph.cbsize = sizeof(wndclassex);
	wcgraph.style = cs_hredraw | cs_vredraw;
	wcgraph.lpfnwndproc = wndgraph;
	wcgraph.hinstance = hinst;
	wcgraph.hcursor = loadcursor(null, idc_cross);
	wcgraph.hbrbackground = (hbrush)(color_window + 1);
	wcgraph.lpszclassname = winclassgraphname;
	wcgraph.hiconsm = loadicon(hinst, makeintresource(idi_icon1));
	return registerclassex(&wcgraph);}
//   function: initinstance(hinstance, int)
//   purpose: saves instance handle and creates main window//   comments:
//        in this function, we save the instance handle in a global variable and
//        create and display the main program window.
bool initinstance(hinstance hinstance, int ncmdshow){  hwnd hwnd;
   hinst = hinstance; // store instance handle in our global variable
   hwnd = createwindow(szwindowclass, sztitle, ws_overlappedwindow,
      cw_usedefault, 0, cw_usedefault, 0, null, null, hinstance, null);
   if (!hwnd)   {      return false;}
showwindow(hwnd, ncmdshow);
   updatewindow(hwnd); return true;}
//  function: wndproc(hwnd, uint, wparam, lparam)
//  purpose:  processes messages for the main window.
//  wm_command	- process the application menu
//  wm_paint	- paint the main window
//  wm_destroy	- post a quit message and return
lresult callback wndproc(hwnd hwnd, uint message, wparam wparam, lparam lparam){
paintstruct ps;// переменная для вывода в окне при обработке сообщения
hdc hdc;// дескриптор контекста устройства
static tchar name[256] = _t("");//статическая переменная для строки
static openfilename file;// структура для организации диалога выбора файла
std::ifstream in;// определение потока ввода
std::ofstream out;// определение потока вывода
static hwnd hgraph//дескриптор
;std::string st;int y, k;
static int n,length,sx,sy,cx,ivscrollpos,ihscrollpos,count,max_width;
static size size = {8, 16 };//ширина и высота символа
static hwnd hwndtoolbar;
static int size_toolbar;rect rt;static char name2[256]=" ";static logfont lf; static choosefont cf;static hfont hfont;textmetric tm;
switch (message){
case wm_create:
enablemenuitem(getmenu(hwnd), id_line, mf_disabled);// блокировка  построения графика
file.lstructsize = sizeof(openfilename);//размер структуры
file.hinstance = hinst;//дескриптор приложения
file.lpstrfilter = _t("text .txt\0 *.txt\0все файлы\0 *.*");//фильтр из пар строк
file.lpstrfile = name;//указатель на строку с именем файла
file.nmaxfile = 256;// размер буфера для имени файла
file.lpstrinitialdir = _t(".\\");//указатель на строку начального каталога
file.lpstrdefext = _t("txt");// указатель на строку с расширением имени файла по умолчанию
hwndtoolbar = createtoolbarex(hwnd,ws_child|ws_visible|ccs_top,1,0,
hinst_commctrl,idb_std_small_color,tbb,3,0,0,0,0,sizeof(tbbutton));//окно дочернее,отображается автоматически,идентификатор окна панели инструмен-в
cf.lstructsize = sizeof(choosefont);//содержит размер структуры choosefont
cf.flags = cf_effects | cf_inittologfontstruct | cf_screenfonts;//определяют флаги cf.hwndowner = hwnd;//определяют дескриптор родительского окна
cf.lplogfont = &lf;break;//хранит указатель на переменную типа logfont
case wm_size:
sx = loword(lparam);// ширина окна
sy = hiword(lparam);// высота окна
k = n - (sy - size_toolbar)/size.cy;// диапазон вертикальной прокрутки
if (k > 0) count = k; else count = ivscrollpos = 0;//определение диапазона вертикального скроллинга
setscrollrange(hwnd, sb_vert, 0, count, false);// установка диапазона 
setscrollpos (hwnd, sb_vert, ivscrollpos, true);// позиция скроллинга
k = length - sx/size.cx;
if (k > 0) max_width = k; else max_width = ihscrollpos = 0;
setscrollrange(hwnd, sb_horz, 0, max_width, false);// установка диапазона
setscrollpos(hwnd, sb_horz, ihscrollpos, true);// позиция скроллинга
sendmessage(hwndtoolbar, tb_autosize, 0, 0);//корректировка размера панели инструментов
getwindowrect(hwndtoolbar, &rt);//координаты окна относительно рабочего стола
size_toolbar = rt.bottom - rt.top;break;
case wm_mousewheel: //прокрутка колесика мыши
ivscrollpos -= (short)hiword(wparam)/wheel_delta; // в старшее слово возвращ кол-во шагов колесика, умножен-е на константу wheel_delta(120).делим это значение на константу и получаем количество шагов
ivscrollpos = max(0, min(ivscrollpos, count));// присваиваем значение, чтобы она не вышла за границы отрезка [0, count]
setscrollpos(hwnd, sb_vert, ivscrollpos, true);//устанав нов позиции чтобы не выйти за границы скрол-га
invalidaterect(hwnd, null, true);//перерисовка окнаbreak;
case wm_vscroll :
switch(loword(wparam)){
case sb_lineup : ivscrollpos--; break;// движок на строку вверх
case sb_linedown : ivscrollpos++; break;// движок на строку вниз 
case sb_pageup : ivscrollpos -= sy/size.cy; break;// движок на страницу вверх
case sb_pagedown : ivscrollpos += sy/size.cy; break;// движок на страницу вниз
case sb_thumbposition : ivscrollpos = hiword(wparam); break;// обработка сообщения о том, что позиция движка была изменена}
ivscrollpos = max(0, min(ivscrollpos, count));
if (ivscrollpos != getscrollpos(hwnd, sb_vert)){
setscrollpos(hwnd, sb_vert, ivscrollpos, true);
invalidaterect(hwnd, null, true);}break;
case wm_hscroll :
switch(loword(wparam)){
case sb_lineup : ihscrollpos--; break;
case sb_linedown : ihscrollpos++; break;
case sb_pageup : ihscrollpos -= 8; break;
case sb_pagedown : ihscrollpos += 8; break;
case sb_thumbposition : ihscrollpos = hiword(wparam); break;}
ihscrollpos = max(0, min(ihscrollpos, max_width));
if (ihscrollpos != getscrollpos(hwnd, sb_horz)){
setscrollpos(hwnd, sb_horz, ihscrollpos, true);
invalidaterect(hwnd, null, true);}break;
case wm_command:switch (loword(wparam))
{//обработка сообщения для выбора пункта меню, когда элемент управления посылает уведомительное сообщение родительскому окну и происходит нажатие клавиши акселератора
case id_file_new :
if (!v.empty()) std::vector<std::string>().swap(v); //очищаем вектор
n = length = 0; //обнуляем переменные описывающие кол-во строк файла и макс длину строки
sendmessage(hwnd, wm_size, 0, sy << 16 | sx); //передаем сообщение окну,в старшее слово помещаем высоту окна с помощью логического сдвига sy << 16 | sx,а в младшее слово 0,т.к. не использ-ся
invalidaterect(hwnd,null,true);break;
case id_file_open :
enablemenuitem(getmenu(hwnd), id_line, mf_enabled);//снятие блокировки с построения графика
file.lpstrtitle = _t("открыть файл для чтения");// выв заголовка диалог окна
file.flags = ofn_hidereadonly;//переключатель только для чтения не выв 
if (!getopenfilename(&file)) return 1;// ;//если работа функц успеш,то в name получ полное имя файла и открываем методом open()
in.open(name);while (getline(in,st)) //читаем построчно файл из потока in чит строку и перемещ в переменную st, пока файл весь не будет прочитан
{if (length < st.length()) length = st.length();//добав строку,по заверш чтения файла в length будет размер строки макс длины	
v.push_back(st);// стр в конец контейн
}in.close();//закрыв входной поток
n = v.size();//получ кол строкфайла как размер контейнера
sendmessage(hwnd, wm_size, 0, sy << 16 | sx);
invalidaterect(hwnd,null,1);break;
case id_file_save :
file.lpstrtitle = _t("открыть файл для записи");
file.flags = ofn_notestfilecreate;
if (!getsavefilename(&file)) return 1;
out.open(name);
for (it = v.begin(); it != v.end(); ++it) out << *it << '\n';//цикл записи в файл содержим контейн от нач до кон контейнр по итератору it,добав в кон кажд стр символ перевода стр
out.close();break;
case id_font :
if(choosefont(&cf))//функция диалога
{if (hfont) deleteobject(hfont);
hfont = createfontindirect(&lf);//создание шрифта
hdc = getdc(hwnd);//получаем контекст устройства
selectobject(hdc, hfont);//устанавливаем шрифт
gettextmetrics(hdc, &tm);//определяем метрику текста
size.cx = tm.tmavecharwidth;//получаем среднюю ширину символа
size.cy = tm.tmheight + tm.tmexternalleading;//высота строки
releasedc(hwnd, hdc);//освобождаем контекст
sendmessage(hwnd, wm_size, 0, sy << 16 | sx);//переопределяем параметры скроллинга
invalidaterect(hwnd, null, true);
}break;case id_line:// пункт меню xy-график
if (iswindow(hgraph)) break;// проверка существования окна
registergraphclass();//регистрация класса окна
hgraph = createwindow(winclassgraphname, _t("xy-график"),
ws_sysmenu | ws_popup | ws_visible | ws_thickframe | ws_caption,sx / 4, sy / 4, min(sx, sy), min(sx, sy), hwnd, 0, hinst, null);break;//стиль окна ограниченного тонкой
рамкой с заголовком  и системным меню и положение всплывающего окна
case idm_exit: destroywindow(hwnd); break;
default: return defwindowproc(hwnd, message, wparam, lparam);}break;
case wm_paint:hdc = beginpaint(hwnd, &ps);// получение контекста уср-ваif (hfont)
{ selectobject(hdc, hfont);
settextcolor(hdc, cf.rgbcolors);
}for (y = size_toolbar, it = v.begin() + ivscrollpos;
it != v.end() && y < sy; ++it, y += size.cy)// переменная y, как и ранее, используется для вычисления y-координаты выводимой строки текста, а итератор it, которым мы выбираем строки текста из контейнера,сместим на ivscrollpos позиций. цикл завершится, если будет достигнут конец контейнера или у-координата строки выйдет за пределы окна.
if (ihscrollpos < it->length())// вывести строку текста с учетом обработки табуляций. выводим строки текста со смещением ihscrollpos, размер строки уменьшается наэту же величину: it->length()-ihscrollpos
tabbedtextouta(hdc, 0, y, it->data()+ihscrollpos,
it->length()-ihscrollpos, 0, null, 0);
endpaint(hwnd, &ps);break;
case wm_destroy:
if (hfont) deleteobject(hfont);
postquitmessage(0);// завершение приложения и передают ос код возврата 0break;
default: return defwindowproc(hwnd, message, wparam, lparam);//если не выполнилась ни одна из альтернатив, то возврат ф-и обработки остальных сообщений по //умолчанию}
return 0;}
const int scalex = 8; //метки по оси x
const int scaley = 4; //метки по оси y
const int indent = 25; //отступ для вывода меток оси х
struct doudle_point { double x, y; };
const int graphsize = 1200;
const int graphwidth = 1000;
lresult callback wndgraph(hwnd hwnd, uint message, wparam wparam, lparam lparam){
	paintstruct ps;
	hdc hdc;
	static hpen hline;
	static hbrush hrect;
	rect rt;
	static int n, sx, sy, kx, ky;
	static double max_x, max_y, min_x, min_y;
	int i, x, y;
	static point *pt;
	tchar s[20];
	doudle_point t;
	double z, hx, hy;
	static doudle_point *xy;
	switch (message)	{case wm_create://создание окна
		if ((n = v.size()) == 0)// проверили загружены ли данные в память{
			messagebox(hwnd, _t("загрузите файл"), _t("нет данных"),
				mb_ok | mb_iconhand);
			destroywindow(hwnd);// уничтожаем определенное окно
			return 1;//возвращаем 1}
		pt = new point[n];
		xy = new doudle_point[n]; // выделили память под массив
		for (it = v.begin(), i = 0; i < n; i++, it++){
if (sscanf_s(it->c_str(), "%lf %lf", &t.x, &t.y) != 2)// читаем цикле функцией sscanf()  извлекаем по два числа из строки{
				messageboxa(hwnd, it->c_str(), "ошибка данных", mb_ok |
					mb_iconhand);
				destroywindow(hwnd);// уничтожаем определенное окно
				return 1;//возвращаем 1}xy[i] = t;}
		max_x = min_x = xy[0].x;// диапазон для x-координат графика
		max_y = min_y = xy[0].y;// диапазон для y-координат графика
		for (i = 1; i < n; i++){
			if (max_x < xy[i].x) max_x = xy[i].x;
			else if (min_x > xy[i].x) min_x = xy[i].x;
			if (max_y < xy[i].y) max_y = xy[i].y;
			else if (min_y > xy[i].y) min_y = xy[i].y;}
		hline = createpen(ps_solid, 6, rgb(0, 0, 255));// создаем перо
		hrect = createsolidbrush(rgb(255, 0, 0));//создаем кисть
		hx = max_x - min_x;hy = max_y - min_y;
		for (i = 0; i < n; i++)	{
			pt[i].x = int((xy[i].x - min_x)*graphwidth / hx + 0.5);
			pt[i].y = int((xy[i].y - min_y)*graphwidth / hy + 0.5);	}break;
	case wm_size:
		sx = loword(lparam);// размеры дочернего окна  sx  
		sy = hiword(lparam);// размеры дочернего окна  sybreak;
	case wm_paint:// вывод графика
		hdc = beginpaint(hwnd, &ps);//контекст устройства
		hx = (max_x - min_x) / scalex;
		hy = (max_y - min_y) / scaley;
		setmapmode(hdc, mm_anisotropic);//setwindowextex(hdc, graphsize, -graphsize, null);//setviewportextex(hdc, sx, sy, null);// установили логическую систему координат 1200 1200 на всю клиентскую часть окна
SetViewportOrgEx(hdc, 2 * indent, sy - indent, NULL);// Начало координат определили относительно исходной системы координат
SetTextAlign(hdc, TA_RIGHT | TA_TOP);// установили выравнивание текста от правой верхней точки
		for (z = min_x, i = 0; i <= scaleX; z += hx, i++){
			x = int((z - min_x)*GRAPHWIDTH / (max_x - min_x) + 0.5);
	_stprintf_s(s, _T("%.1lf"), z);// преобразование числового значения в текстовый вид
			TextOut(hdc, x, 0, s, _tcslen(s));//выводим текст
			MoveToEx(hdc, x, -10, NULL);//устанавливаем текущую позицию
			LineTo(hdc, x, 10);//рисуем линию		}
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, GRAPHWIDTH, 0);
		SetTextAlign(hdc, TA_RIGHT | TA_BOTTOM);
		for (z = min_y, i = 0; i <= scaleY; z += hy, i++)		{
			y = int((z - min_y)*GRAPHWIDTH / (max_y - min_y) + 0.5);
			_stprintf_s(s, _T("%.1lf"), z);
			TextOut(hdc, 0, y, s, _tcslen(s));
			MoveToEx(hdc, -10, y, NULL);
			LineTo(hdc, 10, y);		}MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, 0, GRAPHWIDTH);
		SelectObject(hdc, hline);//устанавливаем  как текущее
		Polyline(hdc, pt, n);//обводим контур
		for (i = 0; i < n; i++)		{
SetRect(&rt, pt[i].x - 8, pt[i].y - 8, pt[i].x + 8, pt[i].y + 8);//задаем поля структур прямоугольника
			FillRect(hdc, &rt, hrect);//заполняем прямоугольник кистью}
		EndPaint(hWnd, &ps);//завершаем обработчик сообщения
		break;	case WM_DESTROY:// освободили занятые ресурсы
		DeleteObject(hline);// удалили перо	DeleteObject(hrect);// удалили кисть 
		delete[] pt;// освободили память, занятую массивами  pt  
		delete[] xy;// освободили память, занятую массивами  xybreak;
	default: return DefWindowProc(hWnd, message, wParam, lParam);}return 0;}
// Message handler for about box.
int_ptr callback about(hwnd hdlg, uint message, wparam wparam, lparam lparam){
	unreferenced_parameter(lparam);switch (message)	{
	case wm_initdialog:return (int_ptr)true;
	case wm_command:if (loword(wparam) == idok || loword(wparam) == idcancel) {enddialog(hdlg, loword(wparam));return (int_ptr)true;}break;	}return (int_ptr)false;}
