#include "pch.h"
#include "main.h"
#include "StringData.h"

#include "Calendar.h"

#include "Cassette.h"
#include "Sheet.h"
#include "File.h"
#include "SQL.h"
#include "OnLine.h"
#include "GdiPlusInit.h"

GUID guidBmp ={};
GUID guidJpeg ={};
GUID guidGif ={};
GUID guidTiff ={};
GUID guidPng ={};

std::string Version = "Программа\r\nКомплекс Подготовки Высокопрочного Листа\r\nВерсия 2.0\r\n\r\nСоздана ";


//std::string lpCassetteDir = "Cassette";
std::string lpLogDir = "Log";

Gdiplus::Font font1(L"Tahoma", 10, Gdiplus::FontStyleBold);
Gdiplus::Font font2(L"Tahoma", 10, Gdiplus::FontStyleRegular);



//Глобальный Instance программы
HINSTANCE hInstance;
HWND GlobalWindow = NULL;
HWND MidiClientWindow = NULL;

std::string szWindowClass0 = "KPVL";
std::string szTitle = "КПВЛ";

std::string CurrentDirPatch = "";
std::string strPatchFileName = "";

#define AllDebugLog "All Debug.log"
std::string FullAllDebugLog  = "All Debug.log";


//Признак работы программы
bool isRun = true;
HACCEL hAccelTable;

//Структура инициализации добавочных контролов
INITCOMMONCONTROLSEX initcontrol ={sizeof(INITCOMMONCONTROLSEX),
      ICC_LISTVIEW_CLASSES    //listview, header
    | ICC_TREEVIEW_CLASSES    // treeview, tooltips
    | ICC_BAR_CLASSES         // toolbar, statusbar, trackbar, tooltips
    | ICC_TAB_CLASSES         // tab, tooltips
    | ICC_UPDOWN_CLASS        // updown
    | ICC_PROGRESS_CLASS      // progress
    | ICC_ANIMATE_CLASS       // animate
    | ICC_WIN95_CLASSES
    | ICC_DATE_CLASSES        // month picker, date picker, time picker, updown
    | ICC_USEREX_CLASSES      // comboex
    | ICC_COOL_CLASSES        // rebar (coolbar) control
    | ICC_INTERNET_CLASSES
    | ICC_PAGESCROLLER_CLASS  // page scroller
    | ICC_NATIVEFNTCTL_CLASS  // native font control
#if (NTDDI_VERSION >= NTDDI_WINXP)
    | ICC_STANDARD_CLASSES
    | ICC_LINK_CLASS
#endif

};

//void CurrentDir();

typedef struct _MYHICON{
    HICON icon;
    int size;
}MYHICON;

extern std::map<int, MYHICON>Icon;


void CheckDir(std::string dir)
{
    struct stat buff;
    if(stat(dir.c_str(), &buff))
        _mkdir(dir.c_str());
}


void CheckAllDir()
{
    CheckDir(lpLogDir);

    FullAllDebugLog = lpLogDir + "\\" + AllDebugLog;
}


//Добавить колонку в ListBox
LRESULT AddColumn(HWND hwndSheet, size_t i, ListTitle* l)
{
    LV_COLUMN lvc ={0};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT | LVCF_SUBITEM;
    lvc.cx = l->cx;
    lvc.pszText = (LPSTR)l->title.c_str();
    lvc.cchTextMax = MAX_STRING;
    lvc.fmt = LVCFMT_CENTER;

    bool b = (bool)ListView_InsertColumn(hwndSheet, i, &lvc);
    return LRESULT(0);
}

//Добавить строку в ListBox
LRESULT AddItem(HWND hwndSheet, bool begin)
{
    LRESULT iItemServ = begin ? 0 : ListView_GetItemCount(hwndSheet);
    LV_ITEM lvi ={0};
    lvi.mask = /*LVIF_PARAM |*/ LVIF_TEXT;
    lvi.pszText = LPSTR_TEXTCALLBACK;// (LPSTR)name;
    lvi.cchTextMax = 255;
    lvi.iItem = (int)iItemServ;
    lvi.lParam = NULL;// param;
    LRESULT ret = ListView_InsertItem(hwndSheet, &lvi);
    return ret;
}


//Закрытие программы
LRESULT Quit()
{
    PostQuitMessage(0);
    return 0;
}

LRESULT Size(LPARAM lParam)
{
    int cx = LOWORD(lParam);
    int cy = HIWORD(lParam);
    MoveWindow(MidiClientWindow, 0, 0, cx, cy, true);
    return 0;
}

std::string Edits = "";
LRESULT OldSubProc = NULL;
LRESULT APIENTRY SubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
       //if(message == WM_CREATE)
       //{
       //	SendMessage(hWnd, WM_SETFONT, SendMessage(GetWindow(hWnd, GW_OWNER), WM_GETFONT, 0, 0), 0);
       //}
       //else
    //VK_LEFT;
    if(message == WM_CHAR)
        switch(wParam)
        {
            case VK_RETURN: //Сохранить
            {
                ULONG len = GetWindowTextLength(hWnd);
                HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, len + 3);
                char* buff = (char*)GlobalLock(hMem);
                GetWindowText(hWnd, buff, len + 2);
                SendMessage(GetParent(hWnd), WM_USER, USER_EDIT_COMMAND, (LPARAM)buff);
            }
            case VK_ESCAPE: //Отмена
                return DestroyWindow(hWnd);
                break;
        }
    if(message == WM_KILLFOCUS)return DestroyWindow(hWnd);

    return CallWindowProc((WNDPROC)OldSubProc, hWnd, message, wParam, lParam);
}


LRESULT OpenKPVL()
{
    OnLineInitInstance();
    return 0;
}

LRESULT OpenSHEET()
{
    SheetInitInstance();
    return 0;
}

LRESULT OpenBASE()
{
    return 0;
}

LRESULT OpenCASSETTE()
{
    CassetteInitInstance();
    return 0;
}



// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch(message)
    {
        case WM_INITDIALOG:
        {
            SetWindowText(GetDlgItem(hDlg, IDC_VERSION), Version.c_str());
            //sprintf_s(sFormat, 100, "%04d-%02d-%02d %02d:%02d. Версия: ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min);
            CenterWindow(hDlg, GlobalWindow);

            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
            if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

LRESULT Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int command = LOWORD(wParam);

    if(command == ID_KPVL) return OpenKPVL();
    if(command == ID_SHEET) return OpenSHEET();
    if(command == ID_CASSETTE) return OpenCASSETTE();
    if(command == ID_BASE) return OpenBASE();

    if(command == IDM_WINDOWTILEH)return SendMessage(MidiClientWindow, WM_MDITILE, MDITILE_HORIZONTAL, 0);
    if(command == IDM_WINDOWTILEV)return SendMessage(MidiClientWindow, WM_MDITILE, MDITILE_VERTICAL, 0);
    if(command == IDM_WINDOWCASCADE)return SendMessage(MidiClientWindow, WM_MDICASCADE, 0, 0);
    if(command == IDM_WINDOWICONS)return SendMessage(MidiClientWindow, WM_MDIICONARRANGE, 0, 0);
    if(command == ID_40026) DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

    if(command == ID_EXIT) return Quit();
    return DefFrameProc(hWnd, MidiClientWindow, message, wParam, lParam);
}



////Перечисление фонтов от 6 до 16
//enum emFont {
//    FontNull = -1,
//    Font06 = 6,
//    Font07 = 7,
//    Font08 = 8,
//    Font09 = 9,
//    Font10 = 10,
//    Font11 = 11,
//    Font12 = 12,
//    Font13 = 13,
//    Font14 = 14,
//    Font15 = 15,
//    Font16 = 16,
//};

//Масив фонтов от 8 до 16 размера жирный шрифт Arial
std::map<emFont, HFONT> Font;

//std::map<int, HICON>Icon;
std::map<int, MYHICON>Icon;


#pragma region Region HBRUSH
//темносиняя заливка
HBRUSH TitleBrush4 = CreateSolidBrush(RGB(0, 99, 177));

//Черная заливка
HBRUSH TitleBrush0 = CreateSolidBrush(RGB(0, 0, 0));

//Белая заливка
HBRUSH TitleBrush1 = CreateSolidBrush(RGB(255, 255, 255));

//синяя заливка
HBRUSH TitleBrush2 = CreateSolidBrush(RGB(177, 99, 177));



//Розовая заливка
HBRUSH TitleBrush3 = CreateSolidBrush(RGB(224, 255, 224));

//синяя заливка
HBRUSH TitleBrush13 = CreateSolidBrush(RGB(0, 177, 177));


//Светло серая заливка
HBRUSH TitleBrush5 = CreateSolidBrush(RGB(245, 245, 245));

//Светло зеленая заливка
HBRUSH TitleBrush6 = CreateSolidBrush(RGB(99, 177, 99));

//Светло желтая заливка
HBRUSH TitleBrush7 = CreateSolidBrush(RGB(255, 255, 192));

//Темно желтая заливка
HBRUSH TitleBrush12 = CreateSolidBrush(RGB(177, 177, 99));

//Светло красная заливка
HBRUSH TitleBrush8 = CreateSolidBrush(RGB(255, 224, 255));

//темно зеленая заливка
HBRUSH TitleBrush9 = CreateSolidBrush(RGB(99, 255, 99));

//Светло красная заливка
HBRUSH TitleBrush10 = CreateSolidBrush(RGB(224, 99, 99));

//Красная заливка
HBRUSH TitleBrush11 = CreateSolidBrush(RGB(224, 0, 0));

//COLORREF m_clrText = 0x00FFFFFF; // (COLORREF)GetStockObject(WHITE_BRUSH);
//COLORREF m_clrTextBk = (COLORREF)GetStockObject(LTGRAY_BRUSH);
#pragma endregion

HRESULT GetGdiplusEncoderClsid(const std::wstring& format, GUID* pGuid)
{
    HRESULT hr = S_OK;
    UINT  nEncoders = 0;          // number of image encoders
    UINT  nSize = 0;              // size of the image encoder array in bytes
    std::vector<BYTE> spData;
    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
    Gdiplus::Status status;
    bool found = false;

    if(format.empty() || !pGuid)
    {
        hr = E_INVALIDARG;
    }

    if(SUCCEEDED(hr))
    {
        *pGuid = GUID_NULL;
        status = Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

        if((status != Gdiplus::Ok) || (nSize == 0))
        {
            hr = E_FAIL;
        }
    }

    if(SUCCEEDED(hr))
    {

        spData.resize(nSize);
        pImageCodecInfo = (Gdiplus::ImageCodecInfo*)&spData.front();
        status = Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

        if(status != Gdiplus::Ok)
        {
            hr = E_FAIL;
        }
    }

    if(SUCCEEDED(hr))
    {
        for(UINT j = 0; j < nEncoders && !found; j++)
        {
            if(pImageCodecInfo[j].MimeType == format)
            {
                *pGuid = pImageCodecInfo[j].Clsid;
                found = true;
            }
        }

        hr = found ? S_OK : E_FAIL;
    }

    return hr;
}


//Инициализация масова фонтов от 8 до 16 размера жирный шрифт Arial
void InitFont()
{
    HDC hDc = GetDC(NULL);
    int DeviceCapsCX = GetDeviceCaps(hDc, LOGPIXELSY);
    int nHeight06 = -MulDiv(6, DeviceCapsCX, 72);
    int nHeight07 = -MulDiv(7, DeviceCapsCX, 72);
    int nHeight08 = -MulDiv(8, DeviceCapsCX, 72);
    int nHeight09 = -MulDiv(9, DeviceCapsCX, 72);
    int nHeight10 = -MulDiv(10, DeviceCapsCX, 72);
    int nHeight11 = -MulDiv(11, DeviceCapsCX, 72);
    int nHeight12 = -MulDiv(12, DeviceCapsCX, 72);
    int nHeight13 = -MulDiv(13, DeviceCapsCX, 72);
    int nHeight14 = -MulDiv(14, DeviceCapsCX, 72);
    int nHeight15 = -MulDiv(15, DeviceCapsCX, 72);
    int nHeight16 = -MulDiv(16, DeviceCapsCX, 72);

    Font[emFont::Font06] = CreateFont(nHeight06, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font07] = CreateFont(nHeight07, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font08] = CreateFont(nHeight08, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font09] = CreateFont(nHeight09, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font10] = CreateFont(nHeight10, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font11] = CreateFont(nHeight11, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font12] = CreateFont(nHeight12, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font13] = CreateFont(nHeight13, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font14] = CreateFont(nHeight14, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font15] = CreateFont(nHeight15, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
    Font[emFont::Font16] = CreateFont(nHeight16, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, 0, "Arial");
}

namespace LISTPAINT{
    LPCTSTR MakeShortString(HDC hdc, LPCTSTR lpszLong, int nColumnLen, int nOffset)
    {
        static TCHAR szThreeDots[]="...";
        SIZE sizeLP;
        int nStringLen=lstrlen(lpszLong);

        GetTextExtentPoint(hdc, lpszLong, nStringLen, &sizeLP);
        if(nStringLen == 0 || sizeLP.cx + nOffset <= nColumnLen)return(lpszLong);

        static TCHAR szShort[MAX_PATH];

        lstrcpy(szShort, lpszLong);

        GetTextExtentPoint(hdc, szThreeDots, sizeof(szThreeDots), &sizeLP);
        int nAddLen=sizeLP.cx;

        for(int i=nStringLen - 1; i > 0; i--)
        {
            szShort[i]=0;
            GetTextExtentPoint(hdc, szShort, i, &sizeLP);
            if(sizeLP.cx + nOffset + nAddLen <= nColumnLen)
                break;
        }

        lstrcat(szShort, szThreeDots);

        return(szShort);
    }

    BOOL GetItemRect(HWND hwnd, int index, RECT* r, TItemRectType2 type)
    {
        r->left = type;
        return SendMessage(hwnd, LVM_GETITEMRECT, index, (LPARAM)r) != 0;
    }

    BOOL GetColumn(HWND hwnd, int index, LV_COLUMN* column)
    {
        return SendMessage(hwnd, LVM_GETCOLUMN, index, (LPARAM)column) != 0;
    }

    LRESULT DrawItem(HWND, UINT, WPARAM, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
        LV_ITEM lvi;
        LV_COLUMN lvc;
        RECT    rc, rcLabel;
        char szBuff[1024];
        LPCTSTR pszText;


        lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
        lvi.iItem=lpdis->itemID;
        lvi.iSubItem=0;
        lvi.pszText=szBuff;
        lvi.cchTextMax=sizeof(szBuff);
        lvi.stateMask=0xFFFF;
        SendMessage(lpdis->hwndItem, LVM_GETITEM, 0, (LPARAM)&lvi);

        BOOL bSelected=lvi.state & LVIS_SELECTED;
        bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

        GetItemRect(lpdis->hwndItem, lpdis->itemID, &rc, Bounds);
        SetBkMode(lpdis->hDC, 1);

        COLORREF clrTextSave = SetTextColor(lpdis->hDC, RGB(0, 0, 0));


        if(bSelected)
        {
            FillRect(lpdis->hDC, &rc, TitleBrush2);
        }
        else
        {
            if(lvi.iItem % 2)
                FillRect(lpdis->hDC, &rc, TitleBrush5);
            else
                FillRect(lpdis->hDC, &rc, TitleBrush1);
        }


        //ImageList_DrawEx(hIL, lvi.iImage, lpdis->hDC, rc.left, rc.top, 16, 16, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT);

        rc.bottom += 1;
        rc.right = rc.left;

        lvc.mask=LVCF_FMT | LVCF_WIDTH;

        SelectObject(lpdis->hDC, Font[emFont::Font10]);

        for(int nColumn=0; GetColumn(lpdis->hwndItem, nColumn, &lvc); nColumn++)
        {
            ListView_GetItemText(lpdis->hwndItem, lpdis->itemID, nColumn, szBuff, sizeof(szBuff));
            //nJustify = DT_LEFT;

            ListView_GetSubItemRect(lpdis->hwndItem, lpdis->itemID, nColumn, LVIR_LABEL, &rcLabel);
            rcLabel.left += 2;
            rcLabel.right -= 2;

            pszText=MakeShortString(lpdis->hDC, szBuff, rcLabel.right - rcLabel.left, 0);

            DrawText(lpdis->hDC, pszText, -1, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
        }


        //if(bSelected)SetBkColor(lpdis->hDC, (ULONG)clrBkSave);
        //SetTextColor(lpdis->hDC, (ULONG)clrTextSave);

        return TRUE;
    }

};

//std::string GetDataTimeStr(std::string str, std::string& outDate, std::string& outTime)
//{
//    outDate = "";
//    outTime = "";
//    boost::regex xRegEx("^(\\d{4})-(\\d{2})-(\\d{2}) (\\d{2}:\\d{2}:\\d{2}).*");
//    boost::match_results<std::string::const_iterator>what;
//    boost::regex_search(str, what, xRegEx, boost::match_default) && what.size();
//    if(what.size() > 4)
//    {
//        std::string year = what[1].str();
//        std::string month = what[2].str();
//        std::string day = what[3].str();
//        if(what[4].length())
//            outTime = what[4].str();
//        if(day.length() && month.length() && year.length())
//            outDate = day + "-" + month + "-" + year;
//    }
//    if(outDate.length() && outTime.length())
//        return outDate + " " + outTime;
//    else
//    {
//        if(outDate.length())
//        {
//            return outDate;
//        }
//        else
//        {
//            if(outTime.length())
//            {
//                outTime;
//            }
//        }
//    }
//    return "";
//}
//std::string GetDataTimeString()
//{
//    std::time_t st = time(NULL);
//    tm curr_tm;
//    localtime_s(&curr_tm, &st);
//
//    std::stringstream sdt;
//    sdt << boost::format("%|04|-") % (curr_tm.tm_year + 1900);
//    sdt << boost::format("%|02|-") % (curr_tm.tm_mon + 1);
//    sdt << boost::format("%|02| ") % curr_tm.tm_mday;
//    sdt << boost::format("%|02|:") % curr_tm.tm_hour;
//    sdt << boost::format("%|02|:") % curr_tm.tm_min;
//    sdt << boost::format("%|02|") % curr_tm.tm_sec;
//    return sdt.str();
//}
//time_t DataTimeOfString(std::string str, std::tm& TM)
//{
//    try
//    {
//        TM.tm_year = 0; TM.tm_mon = 0; TM.tm_mday = 0; TM.tm_hour = 0; TM.tm_min = 0; TM.tm_sec = 0;
//        std::string::const_iterator start = str.begin();
//        std::string::const_iterator end = str.end();
//        boost::match_results<std::string::const_iterator> what;
//        boost::regex xRegEx;
//
//        xRegEx = FORMATTIME1;
//        if(boost::regex_search(start, end, what, xRegEx, boost::match_default) && what.size() > 6)
//        {
//            TM.tm_year = Stoi(what[1]);
//            TM.tm_mon = Stoi(what[2]);
//            TM.tm_mday = Stoi(what[3]);
//            TM.tm_hour = Stoi(what[4]);
//            TM.tm_min = Stoi(what[5]);
//            TM.tm_sec = Stoi(what[6]);
//
//            std::tm tm = TM;
//            tm.tm_year -= 1900;
//            tm.tm_mon -= 1;
//            return mktime(&tm);
//        }
//        else
//        {
//            xRegEx = FORMATTIME2;
//            if(boost::regex_search(start, end, what, xRegEx, boost::match_default))
//            {
//                TM.tm_mday = Stoi(what[1]);
//                TM.tm_mon = Stoi(what[2]);
//                TM.tm_year = Stoi(what[3]);
//                TM.tm_hour = Stoi(what[4]);
//                TM.tm_min = Stoi(what[5]);
//                TM.tm_sec = Stoi(what[6]);
//
//                std::tm tm = TM;
//                tm.tm_year -= 1900;
//                tm.tm_mon -= 1;
//                return mktime(&tm);
//            }
//        }
//    }
//    catch(...) {}
//
//    return 0;
//}
//time_t DataTimeOfString(std::string str)
//{
//    try
//    {
//        std::string::const_iterator start = str.begin();
//        std::string::const_iterator end = str.end();
//        boost::match_results<std::string::const_iterator> what;
//        boost::regex xRegEx;
//
//        xRegEx = FORMATTIME1;
//        if(boost::regex_search(start, end, what, xRegEx, boost::match_default) && what.size() > 6)
//        {
//            std::tm TM;
//            TM.tm_year = Stoi(what[1]) - 1900;
//            TM.tm_mon = Stoi(what[2]) - 1;
//            TM.tm_mday = Stoi(what[3]);
//            TM.tm_hour = Stoi(what[4]);
//            TM.tm_min = Stoi(what[5]);
//            TM.tm_sec = Stoi(what[6]);
//            return mktime(&TM);
//        }
//        else
//        {
//            xRegEx = FORMATTIME2;
//            if(boost::regex_search(start, end, what, xRegEx, boost::match_default) && what.size() > 6)
//            {
//                std::tm TM;
//                TM.tm_mday = Stoi(what[1]);
//                TM.tm_mon = Stoi(what[2]) - 1;
//                TM.tm_year = Stoi(what[3]) - 1900;
//                TM.tm_hour = Stoi(what[4]);
//                TM.tm_min = Stoi(what[5]);
//                TM.tm_sec = Stoi(what[6]);
//                return mktime(&TM);
//            }
//        }
//    }
//    catch(...) {}
//
//    return 0;
//}
//time_t DataTimeDiff(std::string str1, std::string str2)
//{
//    time_t tm1 = DataTimeOfString(str1);
//    time_t tm2 = DataTimeOfString(str2);
//    return (time_t)difftime(tm1, tm2);
//}


//Рисуем заголовок ListBox
LRESULT OnPaintHeadListView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);
    char szBuff[256] = "";
    int count = (int)SendMessageA(hWnd, HDM_GETITEMCOUNT, 0, 0L);
    SelectObject(hDC, TitleBrush1);
    HFONT oldfont = (HFONT)SelectObject(hDC, Font[emFont::Font10]);
    SetTextColor(hDC, RGB(0, 0, 0));
    SetBkMode(hDC, TRANSPARENT);

    for(int i= 0; i < count; i++)
    {
        SIZE siz;
        HDITEM hi ={0};
        RECT rcItem ={0};
        hi.mask = HDI_TEXT;
        hi.pszText = szBuff;
        hi.cchTextMax = 255;
        Header_GetItem(hWnd, i, &hi);
        Header_GetItemRect(hWnd, i, &rcItem);

        FillRect(hDC, &rcItem, TitleBrush2);
        //rcItem.left +=1;
        rcItem.left+=1;
        FillRect(hDC, &rcItem, TitleBrush4);
        FrameRect(hDC, &rcItem, TitleBrush0);

        SetTextColor(hDC, RGB(255, 255, 255));
        SetBkMode(hDC, TRANSPARENT);

        GetTextExtentPoint32A(hDC, szBuff, (int)strlen(szBuff), &siz);
        char* s = szBuff;
        int coco = 1;
        while(s && *s) { if(*(s++) == '\n')coco++; }
        coco *= siz.cy / 2;
        int y = (rcItem.bottom - rcItem.top) / 2;
        rcItem.top = y - coco;
        rcItem.bottom = y + coco;
        DrawTextA(hDC, szBuff, -1, &rcItem, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
    }
    SelectObject(hDC, oldfont);
    EndPaint(hWnd, &ps);
    return 1;
}

//Устанавливаем высоту заголовка ListBox
LRESULT OnHeader_Layout(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    HDLAYOUT* hl = (HDLAYOUT*)lParam;
    LRESULT  ret = DefSubclassProc(hWnd, message, wParam, lParam);

    //hl->pwpos->flags = 0x00000014; //SWP_NOZORDER | SWP_NOACTIVATE
    hl->pwpos->cy = (int)dwRefData;
    hl->prc->top = (int)dwRefData;

    return ret;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_SIZE) return Size(lParam);
    if(message == WM_COMMAND) return Command(hWnd, message, wParam, lParam);
    if(message == WM_DESTROY) if(!Quit())return 0;
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitInstance()
{
    szTitle += " '" + m_dbhost + "'";
#ifdef _DEBUG
    GlobalWindow = CreateWindowExA(0, szWindowClass0.c_str(), (szTitle + " Debug").c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
#else
    GlobalWindow = CreateWindowExA(0, szWindowClass0.c_str(), szTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
#endif // _DEBUG

    if(!GlobalWindow)
        throw std::exception(std::string("Ошибка создания окна : " + szWindowClass0).c_str());

    CLIENTCREATESTRUCT ccs ={0};
    //ccs.hWindowMenu  = GetSubMenu(GetMenu(GlobalWindow), 1);
    //ccs.idFirstChild = IDM_WINDOWICONS + 1;

    MidiClientWindow = CreateWindow("mdiclient", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, 0, 0, 0, 0, GlobalWindow, (HMENU)0xCAC, hInstance, (LPVOID)&ccs);


    if(!MidiClientWindow)
        throw std::exception(std::string("Ошибка создания окна : mdiclient").c_str());


    ShowWindow(GlobalWindow, SW_MAXIMIZE);
    UpdateWindow(GlobalWindow);

    //MdiChildInitInstance();
    SheetInitInstance();
    CassetteInitInstance();
    SendMessage(MidiClientWindow, WM_MDITILE, MDITILE_HORIZONTAL, 0);
}

//Выравнивание окна по центру родительского окна
BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
{
    RECT rcChild, rcParent;
    int  cxChild, cyChild, cxParent, cyParent, cxScreen, cyScreen, xNew, yNew;
    //HDC  hdc;

    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    //hdc = GetDC(NULL);

    int XV = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int YV = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int CXV = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int CYV = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    cxScreen = CXV - XV;
    cyScreen = CYV - YV;
    //cxScreen = GetDeviceCaps(hdc, SM_CXVIRTUALSCREEN);
    //cyScreen = GetDeviceCaps(hdc, SM_CYVIRTUALSCREEN);
    //ReleaseDC(NULL, hdc);

    xNew = rcParent.left + (cxParent - cxChild) / 2;
    if(xNew < 0)xNew = 0; 
    else 
        if((xNew + cxChild) > cxScreen) 
            xNew = cxScreen - cxChild;

    yNew = rcParent.top + (cyParent - cyChild) / 2;
    if(yNew < 0)yNew = 0; 
    else 
        if((yNew + cyChild) > cyScreen) 
            yNew = cyScreen - cyChild;

    return SetWindowPos(hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void DisplayContextMenu(HWND hwnd, int ID)
{
    DWORD dwpos = GetMessagePos();
    POINT pt;
    pt.x = LOWORD(dwpos);
    pt.y = HIWORD(dwpos);

    HMENU hmenu = LoadMenu(hInstance, MAKEINTRESOURCE(ID));
    if(hmenu == NULL) return;

    HMENU hmenuTrackPopup = GetSubMenu(hmenu, 0);
    TrackPopupMenu(hmenuTrackPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    DestroyMenu(hmenu);
}

//Инициализация класса главного окна
void MyRegisterClass()
{
    WNDCLASSEX cex0 ={0};
    cex0.cbSize = sizeof(WNDCLASSEX);
    cex0.style          = CS_HREDRAW | CS_VREDRAW;
    cex0.lpfnWndProc    = WndProc;
    cex0.cbClsExtra     = 0;
    cex0.cbWndExtra     = 0;
    cex0.hInstance      = hInstance;
    cex0.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    cex0.hbrBackground  = (HBRUSH)(CTLCOLOR_DLG + 1);
    cex0.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);
    cex0.lpszClassName  = szWindowClass0.c_str();
#ifndef _DEBUG
    cex0.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    cex0.hIconSm        = cex0.hIcon;
#else
    cex0.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    cex0.hIconSm        = cex0.hIcon;
#endif // _DEBUG
    if(cex0.hInstance && !RegisterClassExA(&cex0))
        throw std::exception("Ошибка регистрации класса окна cex0:");
}

//Вывод строки ошибки выполнения программы
int WinErrorExit(HWND hWnd, const char* lpszFunction)
{
    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf = NULL;
    DWORD dw = GetLastError();

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s\r\nfailed with error %d:\r\n%s"), lpszFunction, dw, (LPTSTR)lpMsgBuf);

    //if(MainLogger) MainLogger->error(std::string((char*)lpDisplayBuf));
    MessageBox(hWnd, (LPCTSTR)lpDisplayBuf, "Error", MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    PostQuitMessage(0);
    return 1;
}


//Глобальная функция
//int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
//{
//    try
//    {
//        hInstance = hInst; // Сохранить маркер экземпляра в глобальной переменной
//        CurrentDir();
//        //DeleteFile(FullAllDebugLog.c_str());
//        InitFont();
//        if(!InitCommonControlsEx(&initcontrol)) throw std::exception("Ошибка загрузки общего модуля");
//
//        BOOL bBool = FALSE;
//        if(!SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)))
//            throw std::exception("Ошибка загрузки пользовательсих модулей");
//
//        SheetInitApplication();
//        CassetteInitApplication();
//        InitClassMont();
//        MyRegisterClass();
//
//        if(InitSQL())
//        {
//            InitInstance();
//            MSG msg;
//
//            while(GetMessage(&msg, nullptr, 0, 0) && msg.message != WM_QUIT)
//            {
//                //if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//                if(!TranslateMDISysAccel(MidiClientWindow, &msg))
//                {
//                    TranslateMessage(&msg);
//                    DispatchMessage(&msg);
//                }
//            }
//        }
//    }
//    catch(std::exception& exc)
//    {
//        WinErrorExit(NULL, exc.what());
//    }
//    catch(...)
//    {
//        WinErrorExit(NULL, "Unknown error.");
//    }
//    EndSQL();
//
//    return 0;
//}

DWORD EnumProcess();
void SavePid();
void ClosePid();

#define SizeLooger 1048576
#define CountLooger 1000

//std::shared_ptr<spdlog::logger> AllLogger= NULL;
//std::shared_ptr<spdlog::logger> InitAllLogger(std::string LoogerOut)
//{
//    try
//    {
//        AllLogger = spdlog::details::registry::instance().get(LoogerOut);
//        if(!AllLogger.get())
//        {
//            std::string FileName = lpLogDir + "/" + LoogerOut + ".log";
//            AllLogger = spdlog::rotating_logger_mt(LoogerOut, FileName, SizeLooger, CountLooger);
//        }
//        AllLogger->set_level(spdlog::level::level_enum::info);
//        return AllLogger;
//    }
//    catch(...) {}
//    return 0;
//}


////Инициализация класса главного окна
//void MyRegisterClass()
//{
//    WNDCLASSEX cex0 ={0};
//    cex0.cbSize = sizeof(WNDCLASSEX);
//    cex0.style          = CS_HREDRAW | CS_VREDRAW;
//    cex0.lpfnWndProc    = WndProc;
//    cex0.cbClsExtra     = 0;
//    cex0.cbWndExtra     = 0;
//    cex0.hInstance      = hInstance;
//    cex0.hCursor        = LoadCursor(nullptr, IDC_ARROW);
//    cex0.hbrBackground  = (HBRUSH)(CTLCOLOR_DLG + 1);
//    cex0.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);
//    cex0.lpszClassName  = szWindowClass0.c_str();
//    cex0.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
//    cex0.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
//
//    if(cex0.hInstance && !RegisterClassExA(&cex0))
//        throw std::exception("Ошибка регистрации класса окна cex0:");
//}



//int Run()
//{
//    try
//    {
//        if(!InitAllLogger("All Debug"))
//        {
//            WinErrorExit(NULL, "Программа уже запущена");
//            return 0;
//        }
//
//        std::string Server = "";
//        DWORD sizeServer = 0;
//
//        GetComputerName(&Server[0], &sizeServer);
//        Server.resize(sizeServer - 1);
//        GetComputerName(&Server[0], &sizeServer);
//
//        if(!hInstance)
//            throw std::runtime_error((FUNCTION_LINE_NAME + std::string("Ошибка запуска программы : hInst = NULL")).c_str());
//
//#ifdef GDIPLUS
//        if(!gbi_plus)
//            throw std::runtime_error(__FUN("Ошибка инициализации библиотеки \"GdiPlus\""));
//#endif
//
//        if(!InitCommonControlsEx(&initcontrol)) throw std::runtime_error(__FUN("Ошибка загрузки общего модуля"));
//
//        BOOL bBool = FALSE;
//
//        if(!SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)))
//            throw std::runtime_error(__FUN("Ошибка загрузки пользовательсих модулей"));
//
//        InitFont();
//
//        SheetInitApplication();
//        CassetteInitApplication();
//        InitClassMont();
//        MyRegisterClass();
//
//        if(InitSQL())
//        {
//            InitInstance();
//            MSG msg;
//
//
//            // Цикл основного сообщения:
//            while(GetMessage(&msg, nullptr, 0, 0) && msg.message != WM_QUIT)
//            {
//                {
//                    if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//                    {
//                        TranslateMessage(&msg);
//                        DispatchMessage(&msg);
//                    }
//                }
//                isRun = false;
//                if(AllLogger)AllLogger->flush();
//            }
//        }
//    }
//    catch(std::runtime_error& exc)
//    {
//        WinErrorExit(NULL, exc.what());
//    }
//    catch(...)
//    {
//        WinErrorExit(NULL, "Unknown error.");
//    }
//    isRun = false;
//
//    //ClosePid();
//
//    return (INT_PTR)FALSE;
//
//
//}

void CurrentDir()
{
    char ss[256] = "";
    GetModuleFileNameA(NULL, ss, 255);
    char ss2[256];
    strcpy_s(ss2, 256, ss);
    char* s1 = ss2;
    char* s2 = NULL;
    while(s1 && *s1)
    {
        if(*s1 == '\\')s2 = s1;
        s1++;
    }
    if(s2)
    {
        *s2 = 0;
        SetCurrentDirectory(ss2);
        CurrentDirPatch = ss2;
    }
    strPatchFileName = std::string(ss);

#ifdef SAWEDEBUG
    remove(AllDebugLog);
#endif
}

std::string MyName = "";
void GetMyName()
{
    DWORD size = 256;
    char buffer[256];
    memset(buffer, 0, size);
    GetComputerName(buffer, &size);
    MyName = buffer;
}

//Глобальная функция
int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        SendDebug("wWinMain", "Start KPVL");
        hInstance = hInst; // Сохранить маркер экземпляра в глобальной переменной
        remove(AllLogger_Rem);

        if(!hInstance)
            throw std::runtime_error((FUNCTION_LINE_NAME + std::string("Ошибка запуска программы : hInstance = NULL")).c_str());

        char* pLockedResource = NULL;
        DWORD dwResourceSize = 0;
        GetRessusce(IDR_TXT1, "TXT", (LPVOID*)&pLockedResource, &dwResourceSize);
        std::string f = pLockedResource;
        boost::replace_all(f, "\r\n", "");

        Version += f;
        //boost::replace_all(Version, "  ", " ");
        //boost::replace_last(Version, " ", "");
        
        GetMyName();
        CurrentDir();
        CheckDir(lpLogDir);

        //if(!InitAllLogger("All Debug"))
        //{
        //    WinErrorExit(NULL, "Программа уже запущена");
        //    return 0;
        //}

        std::string Server = "";
        DWORD sizeServer = 0;

        GetComputerName(&Server[0], &sizeServer);
        Server.resize(sizeServer - 1);
        GetComputerName(&Server[0], &sizeServer);

#ifdef GDIPLUS
        if(!gbi_plus)
            throw std::runtime_error(__FUN("Ошибка инициализации библиотеки \"GdiPlus\""));
#endif
        GetGdiplusEncoderClsid(L"image/bmp", &guidBmp);
        GetGdiplusEncoderClsid(L"image/jpeg", &guidJpeg);
        GetGdiplusEncoderClsid(L"image/gif", &guidGif);
        GetGdiplusEncoderClsid(L"image/tiff", &guidTiff);
        GetGdiplusEncoderClsid(L"image/png", &guidPng);


        if(!InitCommonControlsEx(&initcontrol)) throw std::runtime_error(__FUN("Ошибка загрузки общего модуля"));

        BOOL bBool = FALSE;

        if(!SetUserObjectInformation(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, &bBool, sizeof(BOOL)))
            throw std::runtime_error(__FUN("Ошибка загрузки пользовательсих модулей"));

        if(!LoadRessurse("arial.ttf", IDR_DAT2))throw std::runtime_error(__FUN("Ошибка загрузки файла: \"arial.ttf\""));

        InitFont();

        SheetInitApplication();
        CassetteInitApplication();
        InitClassMont();
        MyRegisterClass();

        if(InitSQL())
        {
            InitInstance();
            MSG msg;


            // Цикл основного сообщения:
            while(GetMessage(&msg, nullptr, 0, 0) && msg.message != WM_QUIT)
            {
                {
                    //if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
                isRun = false;
                //if(AllLogger)AllLogger->flush();
            }
        }
    }
    catch(std::runtime_error& exc)
    {
        WinErrorExit(NULL, exc.what());
    }
    catch(...)
    {
        WinErrorExit(NULL, "Unknown error.");
    }
    isRun = false;

    //ClosePid();

    return (INT_PTR)FALSE;
}
