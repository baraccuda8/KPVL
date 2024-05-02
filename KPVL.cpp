#include "pch.h"
#include <iostream>

#include <strsafe.h>

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "Calendar.h"

#include "Cassette.h"
#include "Sheet.h"
#include "File.h"
#include "SQL.h"
#include "OnLine.h"
#include "GdiPlusInit.h"
#include <gdiplusimagecodec.h>

GUID guidBmp ={};
GUID guidJpeg ={};
GUID guidGif ={};
GUID guidTiff ={};
GUID guidPng ={};


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
    lvi.mask = LVIF_PARAM | LVIF_TEXT;
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


//Черная заливка
HBRUSH TitleBrush0 = CreateSolidBrush(RGB(0, 0, 0));

//Белая заливка
HBRUSH TitleBrush1 = CreateSolidBrush(RGB(255, 255, 255));

//синяя заливка
HBRUSH TitleBrush2 = CreateSolidBrush(RGB(192, 192, 255));

//светлосиняя заливка
HBRUSH TitleBrush3 = CreateSolidBrush(RGB(224, 224, 255));

//темносиняя заливка
HBRUSH TitleBrush4 = CreateSolidBrush(RGB(0, 99, 177));

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

    GlobalWindow = CreateWindowExA(0, szWindowClass0.c_str(), szTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

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
    HDC  hdc;

    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    hdc = GetDC(hwndChild);
    cxScreen = GetDeviceCaps(hdc, HORZRES);
    cyScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

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
    cex0.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    cex0.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

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
    MessageBox(hWnd, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);

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

//Глобальная функция
int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        SendDebug("wWinMain", "Start KPVL");
        hInstance = hInst; // Сохранить маркер экземпляра в глобальной переменной

        if(!hInstance)
            throw std::runtime_error((FUNCTION_LINE_NAME + std::string("Ошибка запуска программы : hInstance = NULL")).c_str());

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
                    if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
