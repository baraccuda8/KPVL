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

#define AllLogger "All Log"
#define AllLogger_Rem (std::string(AllLogger) + ".log").c_str()


//��������� ���������� ������� ���� ListBox
typedef struct {
    std::string title;
    ULONG cx;
}ListTitle;



extern std::string MyName;
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


BOOL CenterWindow(HWND hwndChild, HWND hwndParent);

extern std::map<emFont, HFONT> Font;

extern HINSTANCE hInstance;
extern HWND GlobalWindow;
extern HWND MidiClientWindow;

extern std::string CurrentDirPatch;
extern std::string strPatchFileName;
extern std::string FullAllDebugLog;

#pragma region Region HBRUSH

//������ �������
extern HBRUSH TitleBrush0;

//����� �������
extern HBRUSH TitleBrush1;

//������� �������
extern HBRUSH TitleBrush2;

//����������� �������
extern HBRUSH TitleBrush3;

//���������� �������
extern HBRUSH TitleBrush4;

//������ ����� �������
extern HBRUSH TitleBrush5;

//������ ������� �������
extern HBRUSH TitleBrush6;

//������ ������ �������
extern HBRUSH TitleBrush7;

//������ ������� �������
extern HBRUSH TitleBrush8;

//����� ������� �������
extern HBRUSH TitleBrush9;

//������ ������� �������
extern HBRUSH TitleBrush10;

//������� �������
extern HBRUSH TitleBrush11;

//����� ������ �������
extern HBRUSH TitleBrush12;

//����� �������
extern HBRUSH TitleBrush13;

//extern COLORREF m_clrText;
//extern COLORREF m_clrTextBk;
#pragma endregion


#define ELSEIF(_s, _d) else if(subItem == _s) lstrcpy(plvdi->item.pszText, _d)

void DisplayContextMenu(HWND hwnd, int ID);
LRESULT OnPaintHeadListView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT OnHeader_Layout(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);


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

#define USER_EDIT_COMMAND 1010101
#define USER_COMBO_COMMAND 2010101

extern LRESULT OldSubProc;
LRESULT APIENTRY SubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//inline int Stoi(std::string input)
//{
//    std::optional<int> out = 0;
//    try
//    {
//        return std::stoi(input);
//    }
//    catch(...)
//    {
//    }
//    return 0;
//}
//
//inline float Stof(std::string input)
//{
//    try
//    {
//        return std::stof(input);
//    }
//    catch(...)
//    {
//    }
//    return 0.0f;
//}
//
//std::string GetDataTimeString();
//std::string GetDataTimeStr(std::string str);
//std::string GetDataTimeStr2(std::string str);
//
//time_t DataTimeOfString(std::string str, std::tm& TM);
//time_t DataTimeOfString(std::string str);
//
//time_t DataTimeDiff(std::string str1, std::string str2);
//
//std::string GetDataTimeStr(std::string str, std::string& outDate, std::string& outTime);
//BOOL DataTimeOfString(std::string str, std::string format, std::tm& TM);

