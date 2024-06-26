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
HWND FilterHwndCassette = NULL;

std::string strTitleCassette = "Кассеты";
std::string strClassCassette = "ClassCassette";

std::string DataStartCassette = "";
std::string DataStopCassette = "";
std::string FilterUpdateComand = "SELECT * FROM cassete WHERE finish_at <> to_timestamp('0') ORDER BY create_at DESC;";

//События коссеты
std::map<int, std::string> EventCassette ={
    //{evCassete::Nul,  "Неизвестно"},
    //{evCassete::Fill, "Набирается"},
    //{evCassete::Wait, "Ожидает"},
    //{evCassete::Rel, "Отпуск"},
    //{evCassete::Error, "Авария"},
    //{evCassete::End, "Конец"},
    //{evCassete::History, "Из базы"},
    //{evCassete::Delete, "Удален"}
};

//Заголовки колонок окна кассет
std::map <int, ListTitle> Cassette_Collumn ={
    {Cassete::NN, { "№", 30}},
    {Cassete::Id, { "Ид", 50}},
    {Cassete::Event, { "Событие", 100}},
    {Cassete::Create_at, { "Новая кассета", 130}},
    {Cassete::Year, {"Год", 50}},
    {Cassete::Month, {"Месяц", 50}},
    {Cassete::Day, { "День", 50 }},
    {Cassete::Hour, { "Час", 50 }},


    {Cassete::CassetteNo, { "Касета", 60 }},
    {Cassete::SheetInCassette, { "Листов", 60 }},
    {Cassete::Peth, {"Печь", 50}},
    {Cassete::Run_at, { "Начало отпуска", 130 }},
#ifdef _DEBUG
    {Cassete::Close_at, { "Кассета закрыта", 130 }},
    {Cassete::End_at, { "Конец отпуска", 130 }},
    {Cassete::Delete_at, { "Удалена", 130 }},
#endif
    {Cassete::Finish_at, { "Конец отпуска", 130 }},//{ "Финиш отпуска", 130 }},
    {Cassete::Error_at, { "Ошибка отпуска", 130 }},
    //{Cassete::Delete_at, {"Удален", 130}},
    {Cassete::HeatAcc, {"Факт время\nнагрева", 100 }},
    {Cassete::HeatWait, {"Факт время\nвыдержки", 100 }},
    {Cassete::Total, {"Факт общее\nвремя", 100 }},

    {Cassete::PointRef_1, { "Уставка\nтемпературы", 100 }},
    {Cassete::f_temper, { "Факт\nтемпературы", 100 }},  //Факт температуры за 5 минут до конца отпуска

    {Cassete::PointTime_1, { "Уставка\nвремя нагрева", 130 }},
    {Cassete::TimeProcSet, { "Уставка\nполное время", 130 }},
    {Cassete::PointDTime_2, { "Уставка\nвремя выдержки", 130 }},
#ifdef _DEBUG
    {Cassete::Correct, { "Корректировка", 130 }},
    {Cassete::Pdf, { "Pdf", 130 }},
#endif


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

namespace Coll{
    int Create_at = 0;
    int Id = 0;
    int Event = 0;
    int Day = 0;
    int Month = 0;
    int Year = 0;
    int CassetteNo = 0;
    int SheetInCassette = 0;
    int Close_at = 0;
    int Peth = 0;
    int Run_at = 0;
    int Error_at = 0;
    int End_at = 0;
    int Delete_at = 0;
    int TempRef = 0;           //Заданное значение температуры
    int PointTime_1 = 0;       //Время разгона
    int PointRef_1 = 0;        //Уставка температуры
    int TimeProcSet = 0;       //Полное время процесса (уставка), мин
    int PointDTime_2 = 0;      //Время выдержки
    int f_temper = 0;          //Факт температуры за 5 минут до конца отпуска
    int Finish_at = 0;
    int HeatAcc = 0;           //Факт время нагрева
    int HeatWait = 0;          //Факт время выдержки
    int Total = 0;             //Факт общее время
    int Correct = 0;
    int Pdf = 0;
    int Hour = 0;
};


//Получение номера колонки
void GetCollCassette(PGresult* res)
{
    if(!Coll::Hour)
    {
        int nFields = PQnfields(res);
        for(int j = 0; j < nFields; j++)
        {
            std::string l =  utf8_to_cp1251(PQfname(res, j));
            if(l == "create_at") Coll::Create_at = j;
            else if(l == "id") Coll::Id = j;
            else if(l == "event") Coll::Event = j;
            else if(l == "day") Coll::Day = j;
            else if(l == "month") Coll::Month = j;
            else if(l == "year") Coll::Year = j;
            else if(l == "cassetteno") Coll::CassetteNo = j;
            else if(l == "sheetincassette") Coll::SheetInCassette = j;
            else if(l == "close_at") Coll::Close_at = j;
            else if(l == "peth") Coll::Peth = j;
            else if(l == "run_at") Coll::Run_at = j;
            else if(l == "error_at") Coll::Error_at = j;
            else if(l == "end_at") Coll::End_at = j;
            else if(l == "delete_at") Coll::Delete_at = j;
            else if(l == "tempref") Coll::TempRef = j;
            else if(l == "pointtime_1") Coll::PointTime_1 = j;
            else if(l == "pointref_1") Coll::PointRef_1 = j;
            else if(l == "timeprocset") Coll::TimeProcSet = j;
            else if(l == "pointdtime_2") Coll::PointDTime_2 = j;
            else if(l == "facttemper") Coll::f_temper = j;
            else if(l == "finish_at") Coll::Finish_at = j;
            else if(l == "heatacc")Coll::HeatAcc = j;
            else if(l == "heatwait")Coll::HeatWait = j;
            else if(l == "total")Coll::Total = j;
            else if(l == "correct")Coll::Correct = j;
            else if(l == "pdf")Coll::Pdf = j;
            else if(l == "hour")Coll::Hour = j;
        }
    }
}

//Чтение кассеты
void GetCassette(PGresult* res, TCassette& cassette, int line)
{
    cassette.Create_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Create_at));
    cassette.Id = conn_kpvl.PGgetvalue(res, line, Coll::Id);
    cassette.Event = conn_kpvl.PGgetvalue(res, line, Coll::Event);

    cassette.Hour = conn_kpvl.PGgetvalue(res, line, Coll::Hour);
    cassette.Day = conn_kpvl.PGgetvalue(res, line, Coll::Day);
    cassette.Month = conn_kpvl.PGgetvalue(res, line, Coll::Month);
    cassette.Year = conn_kpvl.PGgetvalue(res, line, Coll::Year);
    cassette.CassetteNo = conn_kpvl.PGgetvalue(res, line, Coll::CassetteNo);
    cassette.SheetInCassette = conn_kpvl.PGgetvalue(res, line, Coll::SheetInCassette);

    cassette.Close_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Close_at));
    cassette.Peth = conn_kpvl.PGgetvalue(res, line, Coll::Peth);
    cassette.Run_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Run_at));
    cassette.Error_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Error_at));
    cassette.End_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::End_at));
    cassette.Delete_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Delete_at));
    cassette.TempRef = conn_kpvl.PGgetvalue(res, line, Coll::TempRef);           //Заданное значение температуры
    cassette.PointTime_1 = conn_kpvl.PGgetvalue(res, line, Coll::PointTime_1);       //Время разгона
    cassette.PointRef_1 = conn_kpvl.PGgetvalue(res, line, Coll::PointRef_1);        //Уставка температуры
    cassette.TimeProcSet = conn_kpvl.PGgetvalue(res, line, Coll::TimeProcSet);       //Полное время процесса (уставка), мин
    cassette.PointDTime_2 = conn_kpvl.PGgetvalue(res, line, Coll::PointDTime_2);      //Время выдержки
    cassette.f_temper = conn_kpvl.PGgetvalue(res, line, Coll::f_temper);          //Факт температуры за 5 минут до конца отпуска
    cassette.Finish_at = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Finish_at)); //Завершение процесса + 15 минут
    cassette.HeatAcc = conn_kpvl.PGgetvalue(res, line, Coll::HeatAcc); //Факт время нагрева
    cassette.HeatWait = conn_kpvl.PGgetvalue(res, line, Coll::HeatWait); //Факт время выдержки
    cassette.Total = conn_kpvl.PGgetvalue(res, line, Coll::Total); //Факт общее время
    cassette.Correct = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Correct));
    cassette.Pdf = GetStringData(conn_kpvl.PGgetvalue(res, line, Coll::Pdf));
}

void FilterUpdate()
{
    //std::stringstream com;
    //com << "DELETE FROM cassette WHERE event = 1 AND id <> (SELECT id FROM cassette WHERE event = 1 ORDER BY id DESC LIMIT 1)";
    //SETUPDATESQL(conn_kpvl, com);

    ListView_DeleteAllItems(ListCassette);
    AllCassette.erase(AllCassette.begin(), AllCassette.end());

    PGresult* res = conn_kpvl.PGexec(FilterUpdateComand);
    //SendDebug("conn_kpvl", FilterUpdateComand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        GetCollCassette(res);

        int line = PQntuples(res);
        //int nFields = PQnfields(res);
        for(int l = 0; l < line; l++)
        {
            TCassette cassette;
            GetCassette(res, cassette, l);
            AllCassette.push_back(cassette);
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
std::string DataFilterCassette;
void FilterDataTimeCassette()
{
    DataFilterCassette = "от " + DataStartCassette + " до " + DataStopCassette;
    SetWindowText(FilterHwndCassette, DataFilterCassette.c_str());

    FilterUpdateComand = "SELECT * FROM cassette ";
    FilterUpdateComand += "WHERE ";
#ifndef _DEBUG
    //FilterUpdateComand += "event = 5 AND "
#endif
        //"finish_at <> to_timestamp('0') AND "
        ;
    //Если не нужны удаленные в ручную то
    //comand += "delete_at IS NULL AND ";
    FilterUpdateComand += "create_at >= '" + DataStartCassette + "' ";
    FilterUpdateComand += "AND create_at <= '" + DataStopCassette + " 23:59:59.999' ";
#ifndef _DEBUG
    //FilterUpdateComand += " AND pdf IS NOT NULL ";
#endif
    FilterUpdateComand += "ORDER BY event, create_at DESC";
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


//Клик правой кнопкой мыши
LRESULT RightClickCassette(LPNM_LISTVIEW pnm)
{
    HWND hwndLV = pnm->hdr.hwndFrom;
    LV_ITEM lvi;
    lvi.iItem = ListView_GetNextItem(hwndLV, -1, LVNI_ALL | LVNI_FOCUSED);
    if(lvi.iItem == -1) return FALSE;
    TCassette& cassette = AllCassette[lvi.iItem];
    //if(cassette.Event != "7")
    //    DisplayContextMenu(hWnd, IDR_MENU2);
    //else
    //    DisplayContextMenu(hWnd, IDR_MENU3);
    return 0;
}

//Двойной клик мыши
LRESULT DoubleClickCassette(LPNM_LISTVIEW pnm)
{
    if(pnm->iItem >= 0 && pnm->iItem < (int)AllCassette.size())
    {
        TCassette& p = AllCassette[pnm->iItem];
        FilterIDCasseteSheet(atoi(p.Year.c_str()), atoi(p.Month.c_str()), atoi(p.Day.c_str()), atoi(p.CassetteNo.c_str()));
    }
    return 0;
}

//Вывод информации кассеты
LRESULT DispInfoCassette(LPARAM lParam)
{
    LV_DISPINFO* plvdi = (LV_DISPINFO*)lParam;
    int item = plvdi->item.iItem;
    int subItem = plvdi->item.iSubItem;
    
   // int icount = ListView_GetItemCount(plvdi->hdr.hwndFrom);

    if(item >= 0 && item < (int)AllCassette.size())
    {
        if(plvdi->item.mask & LVIF_TEXT)
        {
            TCassette& p = AllCassette[item];
            {

                if(plvdi->item.iSubItem == Cassete::NN)                lstrcpy(plvdi->item.pszText, std::to_string(item + 1).c_str());
                if(plvdi->item.iSubItem == Cassete::Id)                lstrcpy(plvdi->item.pszText, p.Id.c_str());
                if(plvdi->item.iSubItem == Cassete::Event)             lstrcpy(plvdi->item.pszText, EventCassette[std::stoi(p.Event)].c_str());
                if(plvdi->item.iSubItem == Cassete::Create_at)         lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Create_at).c_str());

                if(plvdi->item.iSubItem == Cassete::Year)              lstrcpy(plvdi->item.pszText, p.Year.c_str());
                if(plvdi->item.iSubItem == Cassete::Month)             lstrcpy(plvdi->item.pszText, p.Month.c_str());
                if(plvdi->item.iSubItem == Cassete::Day)               lstrcpy(plvdi->item.pszText, p.Day.c_str());
                if(plvdi->item.iSubItem == Cassete::Hour)               lstrcpy(plvdi->item.pszText, p.Hour.c_str());
                if(plvdi->item.iSubItem == Cassete::CassetteNo)        lstrcpy(plvdi->item.pszText, p.CassetteNo.c_str());
                if(plvdi->item.iSubItem == Cassete::SheetInCassette)   lstrcpy(plvdi->item.pszText, p.SheetInCassette.c_str());

#ifdef _DEBUG
                if(plvdi->item.iSubItem == Cassete::Close_at)          lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Close_at).c_str());    //Закрытие касеты
                if(plvdi->item.iSubItem == Cassete::End_at)            lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.End_at).c_str());      //Конец отпуска
                if(plvdi->item.iSubItem == Cassete::Delete_at)         lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Delete_at).c_str());    //Закрытие касеты
#endif
                if(plvdi->item.iSubItem == Cassete::Peth)              lstrcpy(plvdi->item.pszText, p.Peth.c_str());        //Печь
                if(plvdi->item.iSubItem == Cassete::Run_at)            lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Run_at).c_str());      //Начало отпуска
                if(plvdi->item.iSubItem == Cassete::Finish_at)         lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Finish_at).c_str());   //Финиш процесса
                if(plvdi->item.iSubItem == Cassete::Error_at)          lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Error_at).c_str());    //Ошибка отпуска
                if(plvdi->item.iSubItem == Cassete::HeatAcc)           lstrcpy(plvdi->item.pszText, p.HeatAcc.c_str());     //Факт время нагрева
                if(plvdi->item.iSubItem == Cassete::HeatWait)          lstrcpy(plvdi->item.pszText, p.HeatWait.c_str());    //Факт время выдержки
                if(plvdi->item.iSubItem == Cassete::Total)             lstrcpy(plvdi->item.pszText, p.Total.c_str());       //Факт общее время

                if(plvdi->item.iSubItem == Cassete::PointRef_1)        lstrcpy(plvdi->item.pszText, p.PointRef_1.c_str());      //Уставка температуры
                if(plvdi->item.iSubItem == Cassete::f_temper)          lstrcpy(plvdi->item.pszText, p.f_temper.c_str());        //Факт температуры за 5 минут до конца отпуска
                if(plvdi->item.iSubItem == Cassete::PointTime_1)       lstrcpy(plvdi->item.pszText, p.PointTime_1.c_str());     //Уставка время разгона
                if(plvdi->item.iSubItem == Cassete::TimeProcSet)       lstrcpy(plvdi->item.pszText, p.TimeProcSet.c_str());     //Уставка полное время
                if(plvdi->item.iSubItem == Cassete::PointDTime_2)      lstrcpy(plvdi->item.pszText, p.PointDTime_2.c_str());    //Уставка время выдержки
                if(plvdi->item.iSubItem == Cassete::Correct)           lstrcpy(plvdi->item.pszText, GetDataTimeStr(p.Correct).c_str());
                if(plvdi->item.iSubItem == Cassete::Pdf)               lstrcpy(plvdi->item.pszText, p.Pdf.c_str());
            }
        }
    }
    return 0;
}





LRESULT LeftClickCassette(LPNM_LISTVIEW pnm)
{
    if(pnm->iSubItem > 10)
    {
        char szBuff[1024];
        HWND hwndLV = pnm->hdr.hwndFrom;
        LV_ITEM lvi;
        lvi.iItem = ListView_GetNextItem(hwndLV, -1, LVNI_ALL | LVNI_FOCUSED);
        if(lvi.iItem == -1) return FALSE;
        lvi.iSubItem = pnm->iSubItem;
        lvi.pszText = szBuff;
        lvi.cchTextMax = 255;
        lvi.mask = LVIF_TEXT;
        ListView_GetItem(hwndLV, &lvi);

        //int ListSubItem = ((NMLISTVIEW*)pnm)->iSubItem;
        //int ListItem = lvi.iItem;

        RECT r1;
        ListView_GetSubItemRect(hwndLV, lvi.iItem, lvi.iSubItem, LVIR_LABEL, &r1);

        HWND hwEdit = CreateWindowEx(0, "EDIT", szBuff, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top, hwndLV, (HMENU)100, hInstance, 0);
        //SetWindowLong(hwEdit, GWLP_USERDATA, (ULONG)this);
        SendMessage(hwEdit, WM_SETFONT, (WPARAM)Font[emFont::Font09], 0L);

        OldSubProc = SetWindowLongPtr(hwEdit, GWLP_WNDPROC, (LONG_PTR)SubProc);
        SetFocus(hwEdit);
    }
    return 0;
}

LRESULT OnNotifyCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNM_LISTVIEW  pnm = (LPNM_LISTVIEW)lParam;
    if(pnm->hdr.code == NM_RCLICK)return RightClickCassette(pnm);
    else if(pnm->hdr.code == NM_CLICK)return LeftClickCassette(pnm);
    else if(pnm->hdr.code == NM_DBLCLK) return DoubleClickCassette(pnm);
    else if(pnm->hdr.code == LVN_GETDISPINFO)return DispInfoCassette(lParam);

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


LRESULT OldListCassetteSubProc = NULL;
LRESULT ListCassetteSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER && wParam == USER_EDIT_COMMAND && lParam)
    {
        GlobalUnlock((HGLOBAL)lParam);
        GlobalFree((HGLOBAL)lParam);
    }
    //if(message == WM_COMMAND)ListSheetSubCommand(hWnd, wParam, lParam);

    return CallWindowProc((WNDPROC)OldListCassetteSubProc, hWnd, message, wParam, lParam);
}

void InitListCassette()
{
    #define Flag WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/ | LVS_REPORT | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED | LVS_SINGLESEL

    RECT rc;
    GetClientRect(CassetteWindow, &rc);
    ListCassette = CreateWindowEx(0, "SysListView32", "", Flag, 0, 20, rc.right - rc.left, rc.bottom - rc.top - 40, CassetteWindow, (HMENU)hEdit_Sheet, hInstance, nullptr);

    if(!ListCassette)
        throw std::exception(std::string("Ошибка создания окна : ListSheet").c_str());

    OldListCassetteSubProc = SetWindowLongPtr(ListCassette, GWLP_WNDPROC, (LONG_PTR)ListCassetteSubProc);


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
        //SaveCassetteListXlcx(hWnd, SaveFilename, IDR_DAT2);
    }
    return 0;
}

HWND OutDialog = NULL;
DLLRESULT CALLBACK OutInfoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_INITDIALOG)
    {
        OutDialog = GetDlgItem(hWnd, IDC_EDIT1);
        CenterWindow(hWnd, GlobalWindow);
    }
    if(message == WM_CLOSE)    DestroyWindow(hWnd);


    return 0;
}


//std::thread hPdf;
void PrintPdfAuto(TCassette& TC);
//extern std::string FilterComand;
void PrintPdfAuto(TSheet& Sheet, bool view);
void FilterIDCasseteSheet(int stryear, int strmonth, int strday, int strcassetteno);

HANDLE hThreadPdf = NULL;;
HWND hDlg = NULL;

DWORD WINAPI ThreadPdf(LPVOID l)
{
    int iItem = (int)(LRESULT)l;

    TCassette cassette = AllCassette[iItem];
    //FilterIDCasseteSheet(std::stoi(cassette.Year), std::stoi(cassette.Month), std::stoi(cassette.Day), std::stoi(cassette.CassetteNo));
    PrintPdfAuto(cassette);
    //for(auto a : AllSheet)
    //{
    //    PrintPdfAuto(a, false);
    //}
    SendMessage(hDlg, WM_CLOSE, 0, 0);
    hThreadPdf = NULL;
    return 0;
}

LRESULT CommandCassette(HWND hWnd, WPARAM wParam)
{
    WORD command = LOWORD(wParam);
    if(command == ID_POP_40020) //Удалить
    {
        int iItem = ListView_GetNextItem(ListCassette, -1, LVNI_ALL | LVNI_FOCUSED);
        if(iItem >= 0 && iItem < AllCassette.size())
        {
            if(MessageBox(hWnd, "Уверены?", "Удаление", MB_YESNO) == IDYES)
            {
                TCassette& cassette = AllCassette[iItem];
                PGConnection conn;
                conn.connection();
                std::stringstream sd;
                //sd << "UPDATE cassette SET delete_at = now(), event = 7 WHERE id = " << cassette.Id << ";";
                sd << "UPDATE cassette SET delete_at = now() WHERE id = " << cassette.Id << ";";
                SETUPDATESQL(conn, sd);
                UpdateCassette();
            }
        }
    }
    if(command == ID_POP_40021) //Восстановит 
    {
        int iItem = ListView_GetNextItem(ListCassette, -1, LVNI_ALL | LVNI_FOCUSED);
        if(iItem >= 0 && iItem < AllCassette.size())
        {
            if(MessageBox(hWnd, "Уверены?", "Восстановление", MB_YESNO) == IDYES)
            {
                TCassette& cassette = AllCassette[iItem];
                PGConnection conn;
                conn.connection();
                std::stringstream sd;
                sd << "UPDATE cassette SET delete_at = DEFAULT, event = 0 WHERE id = " << cassette.Id << ";";
                SETUPDATESQL(conn, sd);
                UpdateCassette();
            }
        }
    }
    if(command == ID_POP_40023)
    {
        //if(MessageBox(hWnd, "Уверены?", "Паспорта", MB_YESNO) == IDYES)
        //{
        //    hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG5), GlobalWindow, OutInfoProc);
        //    ShowWindow(hDlg, SW_SHOW);
        //
        //    int iItem = ListView_GetNextItem(ListCassette, -1, LVNI_ALL | LVNI_FOCUSED);
        //    if(iItem >= 0 && iItem < AllCassette.size())
        //    {
        //        if(hThreadPdf == NULL)
        //            hThreadPdf = CreateThread(0, 0, ThreadPdf, (LPVOID)(LRESULT)iItem, 0, 0);
        //
        //
        //        //hPdf = std::thread(PrintPdfAuto, AllCassette[iItem]);
        //        //hPdf.join();
        //        //PrintPdfAuto(AllCassette[iItem]);
        //        //std::stringstream ss;
        //        //FilterComand = "SELECT * FROM sheet ";
        //        //FilterComand += "WHERE ";
        //        //FilterComand +=  "year = '" + cassette.Year + "' AND ";
        //        //FilterComand += "month = '" + cassette.Month + "' AND ";
        //        //FilterComand += "day = '" + cassette.Day + "' AND ";
        //        //FilterComand += "cassetteno = '" + cassette.CassetteNo + "' ";
        //
        //        //Год = 2024, Месяц = 4, День = 23, Кассета = 1
        //        //FilterComand 
        //        //});
        //        //th.join();
        //    }
        //}
    }

    if(command == 110) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FilterCassetteDataProc);
    if(command == 111) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FilterCassetteIDProc);;
    if(command == 112) return UpdateCassette();
    if(command == 113) return SaveCassetteList(hWnd);
    return 0;
}

LRESULT DrawItemCassette(HWND, UINT, WPARAM, LPARAM lParam)
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

    if(lvi.iItem >= 0 && lvi.iItem < (int)AllCassette.size())
    {
        TCassette& p = AllCassette[lvi.iItem];

        int Event = Stoi(p.Event);

        BOOL bSelected=lvi.state & LVIS_SELECTED;
        bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

        LISTPAINT::GetItemRect(lpdis->hwndItem, lpdis->itemID, &rc, LISTPAINT::Bounds);
        SetBkMode(lpdis->hDC, 1);

        COLORREF clrTextSave = SetTextColor(lpdis->hDC, RGB(0, 0, 0));

        if(bSelected)
        {
            FillRect(lpdis->hDC, &rc, TitleBrush4);
            clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 255, 255));
        }
        else
        {
            //{evCassete::Nul,  "Неизвестно"},
            //{evCassete::Fill, "Набирается"},
            //{evCassete::Wait, "Ожидает"},
            //{evCassete::Rel, "Отпуск"},
            //{evCassete::Error, "Авария"},
            //{evCassete::End, "Конец"},
            //{evCassete::History, "Из базы"},
            //{evCassete::Delete, "Удален"}

            if(Event == 0) //"Неизвестно"
            {
                FillRect(lpdis->hDC, &rc, TitleBrush10);
            }
            else if(Event == 1) //"Набирается"
            {
                FillRect(lpdis->hDC, &rc, TitleBrush7);
            }
            else if(Event == 2) //"Ожидает"
            {
                //if(lvi.iItem % 2)
                //    FillRect(lpdis->hDC, &rc, TitleBrush5);
                //else
                //    FillRect(lpdis->hDC, &rc, TitleBrush1);
                FillRect(lpdis->hDC, &rc, TitleBrush3);
            }
            else if(Event == 3) //"Отпуск"
            {
                FillRect(lpdis->hDC, &rc, TitleBrush6);
            }
            else if(Event == 4) //"Авария"
            {
                FillRect(lpdis->hDC, &rc, TitleBrush11);
            }
            else if(Event == 5) //"Конец"
            {
                if(lvi.iItem % 2)
                    FillRect(lpdis->hDC, &rc, TitleBrush5);
                else
                    FillRect(lpdis->hDC, &rc, TitleBrush1);
            }
            else if(Event == 6) //"Из базы"
            {
                FillRect(lpdis->hDC, &rc, TitleBrush2);
            }
            else if(Event == 7) //"Удален"
            {
                FillRect(lpdis->hDC, &rc, TitleBrush8);
            }

        }
        rc.bottom += 1;
        rc.right = rc.left;

        lvc.mask=LVCF_FMT | LVCF_WIDTH;

        SelectObject(lpdis->hDC, Font[emFont::Font10]);

        for(int nColumn=0; LISTPAINT::GetColumn(lpdis->hwndItem, nColumn, &lvc); nColumn++)
        {
            ListView_GetItemText(lpdis->hwndItem, lpdis->itemID, nColumn, szBuff, sizeof(szBuff));
            //nJustify = DT_LEFT;

            ListView_GetSubItemRect(lpdis->hwndItem, lpdis->itemID, nColumn, LVIR_LABEL, &rcLabel);
            rcLabel.left += 2;
            rcLabel.right -= 2;

            pszText=LISTPAINT::MakeShortString(lpdis->hDC, szBuff, rcLabel.right - rcLabel.left, 0);

            DrawText(lpdis->hDC, pszText, -1, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
        }

    }
    return TRUE;
}

LRESULT CALLBACK WinProcCassette(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_NOTIFY) return OnNotifyCassette(hWnd, message, wParam, lParam);
    if(message == WM_SIZE)SizeListCassette(lParam);
    if(message == WM_CLOSE)ListCassette = NULL;
    if(message == WM_COMMAND)return CommandCassette(hWnd, wParam);
    if(message == WM_MDIACTIVATE)return ActivateCassette(hWnd, message, wParam, lParam);
    if(message == WM_DRAWITEM)return DrawItemCassette(hWnd, message, wParam, lParam);
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

    FilterHwndCassetteData = CreateWindowEx(0, "BUTTON", "Фильтр по дате", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 0, 0, 150, 20, CassetteWindow, (HMENU)110, hInstance, NULL);
    if(!CassetteWindow)
        throw std::exception(std::string("Ошибка создания окна : Фильтр").c_str());

    //FilterHwndCassetteID = CreateWindowExA(0, "BUTTON", "Касета", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 150, 0, 150, 20, CassetteWindow, (HMENU)111, hInstance, NULL);
    //if(!FilterHwndCassetteID)
    //    throw std::exception(std::string("Ошибка создания окна : Касета").c_str());


    UpdateHwndCassette = CreateWindowEx(0, "BUTTON", "Обновить", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 150, 0, 150, 20, CassetteWindow, (HMENU)112, hInstance, NULL);
    if(!UpdateHwndCassette)
        throw std::exception(std::string("Ошибка создания окна : Обновить").c_str());

    //SaveHwndCassetteList = CreateWindowExA(0, "BUTTON", "Сохранить", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 300, 0, 150, 20, CassetteWindow, (HMENU)113, hInstance, NULL);
    //if(!SaveHwndCassetteList)
    //    throw std::exception(std::string("Ошибка создания окна : Сохранить").c_str());

    FilterHwndCassette = CreateWindowEx(0, "EDIT", DataFilterCassette.c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER | ES_AUTOHSCROLL/* | BS_FLAT*/, 310, 0, 240, 20, CassetteWindow, (HMENU)114, hInstance, NULL);
    if(!FilterHwndCassette)
        throw std::exception(std::string("Ошибка создания окна : Фильтер").c_str());

    //SendMessage(FilterHwndCassetteData, WM_SETFONT, (WPARAM)Font[emFont::Font08], FALSE);
}