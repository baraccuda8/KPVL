#pragma once
#pragma once
#define OPCUACLENT
#define MAX_STRING 1024

#include <windowsx.h>
#include <io.h>
#include <strsafe.h>
#include <shlobj.h>
#include <stdlib.h>
#include <direct.h>
#include <wtsapi32.h>
#include <psapi.h>
#include <userenv.h>
#include <chrono>
#include <commctrl.h>
#include <fstream>

#include "resource.h"

#define SAWEDEBUG

//Перечисление фонтов от 6 до 16
enum emFont {
    FontNull = -1,
    Font06 = 6,
    Font07 = 7,
    Font08 = 8,
    Font09 = 9,
    Font10 = 10,
    Font11 = 11,
    Font12 = 12,
    Font13 = 13,
    Font14 = 14,
    Font15 = 15,
    Font16 = 16,
};

//Вывод строки ошибки выполнения программы
int WinErrorExit(HWND hWnd, const char* lpszFunction);

LRESULT Quit();

//Добавить колонку в ListBox
LRESULT AddColumn(HWND hwndSheet, size_t i, ListTitle* l);
LRESULT AddItem(HWND hwndSheet, bool begin = TRUE);

void CheckDir(std::string dir);

BOOL DataTimeOfString(std::string str, std::string format, std::tm& TM);

BOOL CenterWindow(HWND hwndChild, HWND hwndParent);

extern std::map<emFont, HFONT> Font;

extern HINSTANCE hInstance;
extern HWND GlobalWindow;
extern HWND MidiClientWindow;

extern std::string CurrentDirPatch;
extern std::string strPatchFileName;
extern std::string FullAllDebugLog;

//Черная заливка
extern HBRUSH TitleBrush0;

//Белая заливка
extern HBRUSH TitleBrush1;

//синяя заливка
extern HBRUSH TitleBrush2;

//светлосиняя заливка
extern HBRUSH TitleBrush3;

//темносиняя заливка
extern HBRUSH TitleBrush4;

#define ELSEIF(_s, _d) else if(subItem == _s) lstrcpy(plvdi->item.pszText, _d)

void DisplayContextMenu(HWND hwnd, int ID);

#ifdef _WIN64
#pragma comment(lib, "libxl64.lib")
//#define SIZE_MAX 0xffffffffffffffff
#define DLLRESULT LRESULT
#else
#pragma comment(lib, "libxl32.lib")
//#define SIZE_MAX 0xffffffff
#define DLLRESULT INT_PTR
#endif

