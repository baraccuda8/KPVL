#include "pch.h"
#include "file.h"
#include "main.h"
#include "Calendar.h"
#include "file.h"
#include "Cassette.h"
#include "Sheet.h"

#include "SQL.h"
#include "Exel.h"

std::deque<TCassette>AllCassette;
#define hEdit_Sheet 1001

HWND CassetteWindow = NULL;
HWND ListCassette = NULL;
HWND hHeaderCassette = NULL;
HWND FilterHwndCassetteData = NULL;
HWND FilterHwndCassetteID = NULL;
HWND UpdateHwndCassette = NULL;
HWND SaveHwndCassetteList = NULL;

std::string strTitleCassette = "Кассеты";
std::string strClassCassette = "ClassCassette";

std::string DataStartCassette = "";
std::string DataStopCassette = "";
std::string FilterUpdateComand = "SELECT * FROM cassete ORDER BY create_at DESC;";

//Заголовки колонок окна кассет
std::map <casCassette::cas, ListTitle> Cassette_Collumn ={
    {casCassette::cas::nn, { "№", 30}},
    {casCassette::cas::Id, { "Ид", 50}},
    {casCassette::cas::Create_at, { "Новая кассета", 130}},
    {casCassette::cas::Event, { "Текущая\nпозицитя", 100}},
    {casCassette::cas::Year, {"Год", 50}},
    {casCassette::cas::Month, {"Месяц", 50}},
    {casCassette::cas::Day, { "День", 50 }},
    {casCassette::cas::CassetteNo, { "Касета", 60 }},
    {casCassette::cas::SheetInCassette, { "Листов", 60 }},
    {casCassette::cas::Close_at, { "Кассета закрыта", 130 }},
    {casCassette::cas::Peth, {"Печь", 50}},
    {casCassette::cas::Run_at, { "Начало оптуска", 130 }},
    {casCassette::cas::End_at, { "Конец оптуска", 130 }},
    {casCassette::cas::Error_at, { "Ошибка оптуска", 130 }},
    {casCassette::cas::Delete_at, {"Удален", 130}},

    {casCassette::cas::PointRef_1, { "Уставка\nтемпературы", 100 }},
    {casCassette::cas::f_temper, { "Факт\nтемпературы", 100 }},  //Факт температуры за 5 минут до конца отпуска
    {casCassette::cas::PointTime_1, { "Уставка\nвремя разгона", 130 }},
    {casCassette::cas::TimeProcSet, { "Уставка\nполное время", 130 }},
    {casCassette::cas::PointDTime_2, { "Уставка\nвремя выдержки", 130 }},
     

};

//Заголовки колонок окна касет
std::map<int, const char*> NameEventCassette ={
    {0,  "Неизвестно"},
    {1, "Набирается"},
    {2, "Ожидает"},
    {3, "Отпуск"},
    {4, "Авария"},
    {5, "Конец"},
    {6, "Из базы"},
};


void SizeListCassette(LPARAM lParam)
{
    int cx = LOWORD(lParam);
    int cy = HIWORD(lParam);
    MoveWindow(ListCassette, 0, 20, cx, cy - 40, true);
}

//история кассет
void AddHistoriCassette(bool begin)
{
    AddItem(ListCassette, begin);
}


void FilterUpdate()
{
    ListView_DeleteAllItems(ListCassette);
    AllCassette.erase(AllCassette.begin(), AllCassette.end());

    PGresult* res = conn_kpvl.PGexec(FilterUpdateComand);
    SendDebug("conn_kpvl", FilterUpdateComand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        int nFields = PQnfields(res);
        for(int l = 0; l < line; l++)
        {

            TCassette сassette;

            //create_at | id | event | day | month | year | cassetteno | sheetincassette | close_at | peth | run_at | error_at | end_at | delete_at |
            //std::vector <std::string>split;
            сassette.Create_at = GetStringData(conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::create_at));
            сassette.Id = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::id);
            сassette.Event = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::event);
            сassette.Day = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::day);
            сassette.Month = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::month);
            сassette.Year = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::year);
            сassette.CassetteNo = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::cassetteno);
            сassette.SheetInCassette = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::sheetincassette);
            сassette.Close_at = GetStringData(conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::close_at));
            сassette.Peth = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::peth);
            сassette.Run_at = GetStringData(conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::run_at));
            сassette.Error_at = GetStringData(conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::error_at));
            сassette.End_at = GetStringData(conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::end_at));
            сassette.Delete_at = GetStringData(conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::delete_at));

            сassette.TempRef = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::tempref);
            сassette.PointTime_1 = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::pointtime_1);
            сassette.PointRef_1 = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::pointref_1);
            сassette.TimeProcSet = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::timeprocset);
            сassette.PointDTime_2 = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::pointdtime_2);
            сassette.f_temper = conn_kpvl.PGgetvalue(res, l, casCassette::emCassette::f_temper);


            AllCassette.push_back(сassette);
            AddHistoriCassette(false);
        }
    }
    else
    {
        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        SendDebug("conn_kpvl", errc);
    }
    PQclear(res);
}
void FilterDataTimeCassette()
{
    FilterUpdateComand = "SELECT * FROM cassette ";
    FilterUpdateComand += "WHERE ";
    //Если не нужны удаленные в ручную то
    //comand += "delete_at IS NULL AND ";
    FilterUpdateComand += "create_at >= ";
    FilterUpdateComand += "'" + DataStartCassette + "'";
    FilterUpdateComand += " AND create_at <= ";
    FilterUpdateComand += "'" + DataStopCassette + " 23:59:59.999'";
    FilterUpdateComand += " ORDER BY create_at DESC";
    FilterUpdate();

    //FilterDataTimeCassette();
}

void FilterCassette()
{
    FilterUpdate();
    //ListView_DeleteAllItems(ListCassette);
    //AllCassette.erase(AllCassette.begin(), AllCassette.end());
}

DLLRESULT UserDataCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(wParam == 1)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];
        sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM->wYear, TM->wMonth, TM->wDay);
        DataStartCassette = sFormat;
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), DataStartCassette.c_str());
    }
    if(wParam == 2)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];
        sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM->wYear, TM->wMonth, TM->wDay);
        DataStopCassette = sFormat;
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), DataStopCassette.c_str());
    }
    return 0;
}

DLLRESULT InitFilterCassetteDialog(HWND hWnd)
{
    CenterWindow(hWnd, CassetteWindow);
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), DataStartCassette.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), DataStopCassette.c_str());
    return 0;
}

//IDD_DIALOG3

DLLRESULT CommandFilterCassetteDialog(HWND hWnd, WPARAM wParam)
{
    if(wParam == IDC_BUTTON1)
    {
        CreateCalendar(hWnd, 1);
    }
    else if(wParam == IDC_BUTTON2)
    {
        CreateCalendar(hWnd, 2);
    }

    else if(wParam == IDOK)
    {
        char ss[255];
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT1), ss, 255);
        if(TestData(ss))
            DataStartCassette = ss;
        else
            return MessageBox(hWnd, (std::string(errordata) + ss).c_str(), "Ошибка", 0);

        GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), ss, 255);
        if(TestData(ss))
            DataStopCassette = ss;
        else
            return MessageBox(hWnd, (std::string(errordata) + ss).c_str(), "Ошибка", 0);

        FilterDataTimeCassette();

        EndDialog(hWnd, FALSE);
    }
    else if(wParam == IDCANCEL)
    {
        EndDialog(hWnd, FALSE);
    }
    return 0;
}

DLLRESULT CALLBACK FilterCassetteDataProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER)return UserDataCassette(hWnd, message, wParam, lParam);
    if(message == WM_INITDIALOG)return InitFilterCassetteDialog(hWnd);
    if(message == WM_COMMAND) return CommandFilterCassetteDialog(hWnd, wParam);
    return 0;
}

DLLRESULT CALLBACK FilterCassetteIDProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER)return UserDataCassette(hWnd, message, wParam, lParam);
    if(message == WM_INITDIALOG)return InitFilterCassetteDialog(hWnd);
    if(message == WM_COMMAND) return CommandFilterCassetteDialog(hWnd, wParam);
    return 0;
}

LRESULT OnNotifyCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNM_LISTVIEW  pnm = (LPNM_LISTVIEW)lParam;
    if(pnm->hdr.code == NM_DBLCLK)
    {
        LPNMITEMACTIVATE lvi = (LPNMITEMACTIVATE)lParam;
        if(lvi->iItem >= 0 && lvi->iItem < (int)AllCassette.size())
        {
            TCassette& p = AllCassette[lvi->iItem];
            FilterIDCasseteSheet(atoi(p.Year.c_str()), atoi(p.Month.c_str()), atoi(p.Day.c_str()), atoi(p.CassetteNo.c_str()));
        }
    }
    else if(pnm->hdr.code == LVN_GETDISPINFO)
    {
        LV_DISPINFO* plvdi = (LV_DISPINFO*)lParam;

        int item = plvdi->item.iItem;
        int subItem = plvdi->item.iSubItem;
        if(item >= 0 && item < (int)AllCassette.size())
        {
            if(plvdi->item.mask & LVIF_TEXT)
            {
                TCassette& p = AllCassette[item];
                {

                    ////{casCassette::cas::nn, {"№", 30} },
                    ////{casCassette::cas::Id, { "Ид", 50}},
                    ////{casCassette::cas::Create_at, { "Новая кассета", 130}},
                    ////{casCassette::cas::Event, { "Текущая\nпозицитя", 100}},
                    ////{casCassette::cas::Year, {"Год", 50}},
                    ////{casCassette::cas::Month, {"Месяц", 50}},
                    ////{casCassette::cas::Day, { "День", 50 }},
                    ////{casCassette::cas::CassetteNo, { "Касета", 60 }},
                    ////{casCassette::cas::SheetInCassette, { "Листов", 60 }},
                    ////{casCassette::cas::Close_at, { "Кассета закрыта", 130 }},
                    ////{casCassette::cas::Peth, {"Печь", 50}},
                    ////{casCassette::cas::Run_at, { "Начало оптуска", 130 }},
                    ////{casCassette::cas::End_at, { "Конец оптуска", 130 }},
                    ////{casCassette::cas::Error_at, { "Ошибка оптуска", 130 }},
                    ////{casCassette::cas::Delete_at, {"Удален", 130}},

                    ////{casCassette::cas::TempRef, { "Заданная\nтемпература", 80 }},            //Заданное значение температуры
                    //{casCassette::cas::PointTime_1, { "Время\nразгона", 80 }},
                    //{casCassette::cas::PointRef_1, { "Уставка\nтемпературы", 80 }},
                    //{casCassette::cas::TimeProcSet, { "Полное\nвремя", 80 }},
                    //{casCassette::cas::PointDTime_2, { "Время\nвыдержки", 80 }},
                    //{casCassette::cas::f_temper, { "Факт\nтемпературы", 88 }},  //Факт температуры за 5 минут до конца отпуска

                    if(subItem == casCassette::nn)          lstrcpy(plvdi->item.pszText, std::to_string(item + 1).c_str());
                    else if(subItem == casCassette::Id) lstrcpyA(plvdi->item.pszText, p.Id.c_str());
                    else if(subItem == casCassette::Create_at) lstrcpyA(plvdi->item.pszText, p.Create_at.c_str());
                    else if(subItem == casCassette::Event) lstrcpyA(plvdi->item.pszText, NameEventCassette[atoi(p.Event.c_str())]);
                    else if(subItem == casCassette::Year) lstrcpyA(plvdi->item.pszText, p.Year.c_str());
                    else if(subItem == casCassette::Month) lstrcpyA(plvdi->item.pszText, p.Month.c_str());
                    else if(subItem == casCassette::Day) lstrcpyA(plvdi->item.pszText, p.Day.c_str());
                    else if(subItem == casCassette::CassetteNo) lstrcpyA(plvdi->item.pszText, p.CassetteNo.c_str());
                    else if(subItem == casCassette::SheetInCassette) lstrcpyA(plvdi->item.pszText, p.SheetInCassette.c_str());
                    else if(subItem == casCassette::Close_at) lstrcpyA(plvdi->item.pszText, p.Close_at.c_str());
                    else if(subItem == casCassette::Peth) lstrcpyA(plvdi->item.pszText, p.Peth.c_str());
                    else if(subItem == casCassette::Run_at) lstrcpyA(plvdi->item.pszText, p.Run_at.c_str());
                    else if(subItem == casCassette::End_at) lstrcpyA(plvdi->item.pszText, p.End_at.c_str());
                    else if(subItem == casCassette::Error_at) lstrcpyA(plvdi->item.pszText, p.Error_at.c_str());
                    else if(subItem == casCassette::Delete_at) lstrcpyA(plvdi->item.pszText, p.Delete_at.c_str());
                    //else if(subItem == casCassette::TempRef) lstrcpyA(plvdi->item.pszText, p.TempRef.c_str());
                    else if(subItem == casCassette::PointTime_1) lstrcpyA(plvdi->item.pszText, p.PointTime_1.c_str());
                    else if(subItem == casCassette::PointRef_1) lstrcpyA(plvdi->item.pszText, p.PointRef_1.c_str());
                    else if(subItem == casCassette::TimeProcSet) lstrcpyA(plvdi->item.pszText, p.TimeProcSet.c_str());
                    else if(subItem == casCassette::PointDTime_2) lstrcpyA(plvdi->item.pszText, p.PointDTime_2.c_str());
                    else 
                        if(subItem == casCassette::f_temper) 
                            lstrcpyA(plvdi->item.pszText, p.f_temper.c_str());


                }
            }
        }
    }
    return 0;
}

LRESULT ActivateCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //HWND activate = (HWND)wParam;
    CheckMenuItem(GetMenu(GlobalWindow), ID_CASSETTE, (lParam == (LPARAM)hWnd) ? MF_CHECKED : MF_UNCHECKED);
    //SendMessage(MidiClientWindow, WM_MDISETMENU, (WPARAM)hMenuInit, (LPARAM)hMenuInitWindow);
    return 0;
}


LRESULT OnPaintHeadListView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT OnHeader_Layout(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

//Обработека сообщений заголовка ListBpx Рисуем заголовок
LRESULT CALLBACK WndProcHeadListViewCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    if(message == WM_ERASEBKGND)    return 0;
    if(message == WM_PAINT)         return OnPaintHeadListView(hWnd, message, wParam, lParam, uIdSubclass, dwRefData);
    if(message == HDM_LAYOUT)       return OnHeader_Layout(hWnd, message, wParam, lParam, uIdSubclass, dwRefData);

    return DefSubclassProc(hWnd, message, wParam, lParam);
}

void InitListCassette()
{
    #define Flag WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/ | LVS_REPORT | LVS_NOSORTHEADER

    RECT rc;
    GetClientRect(CassetteWindow, &rc);
    ListCassette = CreateWindowEx(0, "SysListView32", "", Flag, 0, 20, rc.right - rc.left, rc.bottom - rc.top - 40, CassetteWindow, (HMENU)hEdit_Sheet, hInstance, nullptr);

    if(!ListCassette)
        throw std::exception(std::string("Ошибка создания окна : ListSheet").c_str());


    hHeaderCassette = ListView_GetHeader(ListCassette);

    SetWindowSubclass(hHeaderCassette, WndProcHeadListViewCassette, 1, 32);

    SendMessage(ListCassette, WM_SETFONT, (WPARAM)Font[emFont::Font09], FALSE);
    ListView_SetExtendedListViewStyle(ListCassette, LVS_EX_HEADERINALLVIEWS | LVS_EX_DOUBLEBUFFER | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_REGIONAL | LVS_EX_FULLROWSELECT);

    for(auto a : Cassette_Collumn | boost::adaptors::indexed(0))
        AddColumn(ListCassette, a.index(), &a.value().second);

    FilterDataTimeCassette();

    #undef Flag

}

LRESULT UpdateCassette()
{
    FilterCassette();
    return 0;
}

//Сохранить список кассет в Exel файл
LRESULT SaveCassetteList(HWND hWnd)
{
    std::string file;
    FilterUpdate();

    //if(bFilterData)
        file = "Cassette " + DataStartCassette + " " + DataStopCassette;
    //else
    //    file =  strYear + "-" + strMonth + "-" + strDay + " " + strCassetteNo;

    char SaveFilename[MAX_STRING];
    strcpy_s(SaveFilename, 255, file.c_str());
    if(CmdFileSaveXlsx(hWnd, SaveFilename))
    {
        //MessageBox(hWnd, SaveFilename, "", 0);
        SaveCassetteListXlcx(hWnd, SaveFilename, IDR_DAT2);
    }
    return 0;
}

LRESULT CommandCassette(HWND hWnd, WPARAM wParam)
{
    WORD command = LOWORD(wParam);
    if(command == 110) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FilterCassetteDataProc);
    if(command == 111) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FilterCassetteIDProc);;
    if(command == 112) return UpdateCassette();
    if(command == 113) return SaveCassetteList(hWnd);
    return 0;
}


LRESULT CALLBACK WinProcCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_NOTIFY) return OnNotifyCassette(hWnd, message, wParam, lParam);
    if(message == WM_SIZE)SizeListCassette(lParam);
    if(message == WM_CLOSE)ListCassette = NULL;
    if(message == WM_COMMAND)return CommandCassette(hWnd, wParam);
    if(message == WM_MDIACTIVATE)return ActivateCassette(hWnd, message, wParam, lParam);
    return DefMDIChildProc(hWnd, message, wParam, lParam);
}

void CassetteInitApplication()
{
    WNDCLASSEX wc ={0};

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)WinProcCassette;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = strClassCassette.c_str();
    wc.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClassEx(&wc);

}

void CassetteInitInstance()
{
    std::tm TM;

    time_t timer = time(NULL);
    localtime_s(&TM, &timer);

    char sFormat[1024];
    sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday);
    DataStopCassette = sFormat;

    timer = (time_t)difftime(timer, 60 * 60 * 24); // минус 1 день //4 месяца = 10540800
    localtime_s(&TM, &timer);
    sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday);
    DataStartCassette = sFormat;

    if(CassetteWindow)
    {
        SendMessage(MidiClientWindow, WM_MDIACTIVATE, (WPARAM)CassetteWindow, 0);
        return;
    }

    MDICREATESTRUCT mcs ={0};
    mcs.szTitle = strTitleCassette.c_str();
    mcs.szClass = strClassCassette.c_str();
    mcs.hOwner  = hInstance;
    mcs.x = mcs.cx = CW_USEDEFAULT;
    mcs.y = mcs.cy = CW_USEDEFAULT;
    mcs.style = WS_MAXIMIZE | MDIS_ALLCHILDSTYLES;

    CassetteWindow = (HWND)SendMessage(MidiClientWindow, WM_MDICREATE, 0, (LPARAM)&mcs);
    if(!CassetteWindow)
        throw std::exception(std::string("Ошибка создания окна : CassetteWindow").c_str());

    InitListCassette();

    FilterHwndCassetteData = CreateWindowExA(0, "BUTTON", "Фильтр по дате", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 0, 0, 150, 20, CassetteWindow, (HMENU)110, hInstance, NULL);
    if(!CassetteWindow)
        throw std::exception(std::string("Ошибка создания окна : Фильтр").c_str());

    //FilterHwndCassetteID = CreateWindowExA(0, "BUTTON", "Касета", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 150, 0, 150, 20, CassetteWindow, (HMENU)111, hInstance, NULL);
    //if(!FilterHwndCassetteID)
    //    throw std::exception(std::string("Ошибка создания окна : Касета").c_str());


    UpdateHwndCassette = CreateWindowExA(0, "BUTTON", "Обновить", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 150, 0, 150, 20, CassetteWindow, (HMENU)112, hInstance, NULL);
    if(!UpdateHwndCassette)
        throw std::exception(std::string("Ошибка создания окна : Обновить").c_str());

    SaveHwndCassetteList = CreateWindowExA(0, "BUTTON", "Сохранить", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 300, 0, 150, 20, CassetteWindow, (HMENU)113, hInstance, NULL);
    if(!SaveHwndCassetteList)
        throw std::exception(std::string("Ошибка создания окна : Сохранить").c_str());

    //SendMessage(FilterHwndCassetteData, WM_SETFONT, (WPARAM)Font[emFont::Font08], FALSE);
}