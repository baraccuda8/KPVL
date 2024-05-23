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

//������������ ������ �� 6 �� 16
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

//����� ������ ������ ���������� ���������
int WinErrorExit(HWND hWnd, const char* lpszFunction);

LRESULT Quit();

//�������� ������� � ListBox
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

//������ �������
extern HBRUSH TitleBrush0;

//����� �������
extern HBRUSH TitleBrush1;

//����� �������
extern HBRUSH TitleBrush2;

//����������� �������
extern HBRUSH TitleBrush3;

//���������� �������
extern HBRUSH TitleBrush4;

//extern COLORREF m_clrText;
//extern COLORREF m_clrTextBk;


#define ELSEIF(_s, _d) else if(subItem == _s) lstrcpy(plvdi->item.pszText, _d)

void DisplayContextMenu(HWND hwnd, int ID);

namespace LISTPAINT{
    enum TItemRectType2{
        Bounds        = LVIR_BOUNDS,     // Entire boundary (icon and label)
        Icons         = LVIR_ICON,       // Only the icon
        Label         = LVIR_LABEL,      // Only the label
        SelectBounds  = LVIR_SELECTBOUNDS// Union of Icon+Label but no colmns
    };


    LPCTSTR MakeShortString(HDC hdc, LPCTSTR lpszLong, int nColumnLen, int nOffset);
    BOOL GetItemRect(HWND hwnd, int index, RECT* r, TItemRectType2 type);
    BOOL GetColumn(HWND hwnd, int index, LV_COLUMN* column);
    LRESULT DrawItem(HWND, UINT, WPARAM, LPARAM lParam);

}