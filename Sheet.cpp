#include "pch.h"

#include "main.h"


#include "Calendar.h"
#include "file.h"
#include "Cassette.h"
#include "Sheet.h"

#include "SQL.h"
#include "Exel.h"


//Класс окна Static
#define szStat "Static"

LRESULT CALLBACK WndProcHeadListViewSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

typedef struct qwe
{
    std::string n;
    LRESULT t;
    float f;
}qwe;

std::map<std::string, std::deque<qwe>>DataTempSheet;
std::map<std::string, std::deque<qwe>>DataTempCassette;

std::string command = "SELECT create_at, content, FROM todos WHERTE id = 13 ORDER BY create_at;";

std::deque<TSheet>AllSheet;

#define hEdit_Sheet 1001

HWND SheetWindow = NULL;
HWND ListSheet = NULL;
HWND hHeaderSheet = NULL;
HWND FilterHwndSheetData = NULL;
HWND FilterHwndSheetID = NULL;
HWND UpdateHwndSheet = NULL;
HWND SaveHwndSheetList = NULL;
HWND ListSheetTemp = NULL;
HWND ListCasseteTemp = NULL;

std::string strTitleSheet = "Лист";
std::string strClassSheet = "ClassSheet";

std::string DataStartSheet = "";
std::string DataStopSheet = "";

#define LL0 130
#define LL1 100
#define LL2 90
#define L0 50
#define L1 60
#define L2 70

std::map <int, ListTitle> CasseteTemp_Collumn ={
    {0,  { "№", 30 }},
    {1,  { "Дата время", LL0 }},
    {2,  { "Задание", LL2 }},
    {3,  { "Факт", LL2 }},
    {4,  { "T 1", LL2 }},
    {5,  { "Т 2", LL2 }},
};

std::map <int, ListTitle> SheetTemp_Collumn ={
    {0,  { "№", 30 }},
    {1,  { "Дата время", LL0 }},
    {2,  { "Уставка", L0 }},
    {3,  { "Пиром", L0 }},
    {4,  { "Т 1.1", L0 }},
    {5,  { "Т 1.2", L0 }},
    {6,  { "Т 1.3", L0 }},
    {7,  { "Т 1.4", L0 }},
    {8,  { "Т 2.1", L0 }},
    {9,  { "Т 2.2", L0 }},
    {10,  { "Т 2.3", L0 }},
    {11, { "Т 2.4", L0 }},
};

//Заголовки колонок окна листов
std::map <casSheet::cas, ListTitle> Sheet_Collumn ={
    {casSheet::NN, { "№", 30 }},
    {casSheet::ID, { "ID", 50 }},
    {casSheet::DataTime, { "Дата время\nсоздания листа", LL0 }},
    {casSheet::Start_at, { "Дата время\nзагрузки в печь", LL0 }},
    //{casSheet::Zone, { "Лист\nнайден в", 100 }},
    {casSheet::Pos, { "Текущая\nпозиция", 100 }},
    {casSheet::News, { "Кантовка", L2 }},

    {casSheet::DataTime_End, { "Дата время\nвыгрузки из печи", LL0 }},
    {casSheet::DataTime_All, { "Время закалки\nмин", L1 }},
    {casSheet::TimeForPlateHeat, { "Задание Время\nокончания нагрева", L2 }},
    {casSheet::PresToStartComp, { "Задание\nДавления воды", L2 }},

    {casSheet::Alloy, { "Марка стали", L1 }},
    {casSheet::Thikness, { "Толщина\nлиста\nмм", L2 }},
    {casSheet::Melt, { "Плавка", L1 }},
    {casSheet::PartNo, { "Партия", L0 }},
    {casSheet::Slab, { "Сляб", L0 }},
    {casSheet::Pack, { "Пачка", L0 }},
    {casSheet::Sheet, { "Номер\nлиста", L0 }},
    {casSheet::SubSheet, { "Номер\nдодлиста", L0 }},
    {casSheet::Temper, { "Заданная\nтемпература\nС°", LL2 }},
    {casSheet::Speed, { "Скорость\nвыдачи\nмм/с", 80 }},
    {casSheet::Za_PT3, { "Давление\nводы в баке.\nбар", LL2 }},
    {casSheet::Za_TE3, { "Температура\nводы в баке.\nС°", LL1 }},
    {casSheet::LamPressTop, { "Давление\nв верхнем\nколлекторе", LL2 }},
    {casSheet::LamPressBot, { "Давление\nв нижнем\nколлекторе", LL2 }},
    {casSheet::PosClapanTop, { "Скоростная\nклапан верх", LL2 }},
    {casSheet::PosClapanBot, { "Скоростная\nклапан низ", LL2 }},
    {casSheet::Mask, { "Маскирование", 140 }},
    {casSheet::Lam1PosClapanTop, { "Ламинарная 1\nКлапан верх", LL1 }},
    {casSheet::Lam1PosClapanBot, { "Ламинарная 1\nКлапан низ", LL1 }},
    {casSheet::Lam2PosClapanTop, { "Ламинарная 2\nКлапан верх", LL1 }},
    {casSheet::Lam2PosClapanBot, { "Ламинарная 2\nКлапан низ", LL1 }},

    {casSheet::Lam_TE1, { "Температура\nводы\nв поддоне. С°", LL1 }},

    {casSheet::Day, { "ID листа\nДень", L1 }},
    {casSheet::Month, { "ID листа\nМесяц", L1 }},
    {casSheet::Year, { "ID листа\nГод", L1 }},
    {casSheet::CassetteNo, { "ID листа\nКасета", L1 }},
    {casSheet::SheetInCassette, { "Id листа\nНомер", L1 }},

    {casSheet::Top1, { "До\nкантовки\n1", L2 }},
    {casSheet::Top2, { "До\nкантовки\n2", L2 }},
    {casSheet::Top3, { "До\nкантовки\n3", L2 }},
    {casSheet::Top4, { "До\nкантовки\n4", L2 }},
    {casSheet::Top5, { "До\nкантовки\n5", L2 }},
    {casSheet::Top6, { "До\nкантовки\n6", L2 }},
    {casSheet::Top7, { "До\nкантовки\n7", L2 }},
    {casSheet::Top8, { "До\nкантовки\n8", L2 }},

    {casSheet::Bot1, { "После\nкантовки\n1", L2 }},
    {casSheet::Bot2, { "После\nкантовки\n2", L2 }},
    {casSheet::Bot3, { "После\nкантовки\n3", L2 }},
    {casSheet::Bot4, { "После\nкантовки\n4", L2 }},
    {casSheet::Bot5, { "После\nкантовки\n5", L2 }},
    {casSheet::Bot6, { "После\nкантовки\n6", L2 }},
    {casSheet::Bot7, { "После\nкантовки\n7", L2 }},
    {casSheet::Bot8, { "После\nкантовки\n8", L2 }},
};

//bool SortSheet(std::pair<int, ListTitle>* d1, std::pair <int, ListTitle>* d2)
//{
//    return d1->first < d2->first;
//}

void SizeListSheet(HWND List, LPARAM lParam)
{
    int cx = LOWORD(lParam);
    int cy = HIWORD(lParam);
    MoveWindow(List, 0, 20, cx, cy - 20, true);
    //MoveWindow(SaveHwndSheetList, cx-150, 0, 150, 20, true);
    
}

//история кантовки листа
void AddHistoriSheet(bool begin)
{
    //int item = ListView_GetItemCount(ListSheet);
    //if(item <= KPVLTYPE.CountLastSheet)
    AddItem(ListSheet, begin);
    //else
    //    InvalidateRect(hwndSheet, NULL, FALSE);
}

std::string FilterComand = "SELECT * FROM sheet ORDER BY create_at DESC;";

namespace {
    enum{
        Col_Sheet_id = 0,
        Col_Sheet_create_at = 1,
        Col_Sheet_start_at = 2,
        Col_Sheet_datatime_end = 3,
        Col_Sheet_pos = 4,
        Col_Sheet_datatime_all,
        Col_Sheet_alloy,
        Col_Sheet_thikness,
        Col_Sheet_melt,
        Col_Sheet_slab,
        Col_Sheet_partno,
        Col_Sheet_pack,
        Col_Sheet_sheet,
        Col_Sheet_subsheet,
        Col_Sheet_temper,
        Col_Sheet_speed,
        Col_Sheet_za_pt3,
        Col_Sheet_za_te3,
        Col_Sheet_lampresstop,
        Col_Sheet_lampressbot,
        Col_Sheet_posclapantop,
        Col_Sheet_posclapanbot,
        Col_Sheet_mask,
        Col_Sheet_lam1posclapantop,
        Col_Sheet_lam1posclapanbot,
        Col_Sheet_lam2posclapantop,
        Col_Sheet_lam2posclapanbot,
        Col_Sheet_lam_te1,
        Col_Sheet_news,
        Col_Sheet_top1,
        Col_Sheet_top2,
        Col_Sheet_top3,
        Col_Sheet_top4,
        Col_Sheet_top5,
        Col_Sheet_top6,
        Col_Sheet_top7,
        Col_Sheet_top8,
        Col_Sheet_bot1,
        Col_Sheet_bot2,
        Col_Sheet_bot3,
        Col_Sheet_bot4,
        Col_Sheet_bot5,
        Col_Sheet_bot6,
        Col_Sheet_bot7,
        Col_Sheet_bot8,
        Col_Sheet_day,
        Col_Sheet_month,
        Col_Sheet_year,
        Col_Sheet_cassetteno,
        Col_Sheet_sheetincassette,
        Col_Sheet_timeforplateheat,
        Col_Sheet_prestostartcomp,
    };

    //int Col_Sheet_id = 0;
    //int Col_Sheet_create_at = 0;
    //int Col_Sheet_start_at = 0;
    //int Col_Sheet_datatime_end = 0;
    //int Col_Sheet_datatime_all = 0;
    //int Col_Sheet_alloy = 0;
    //int Col_Sheet_thikness = 0;
    //int Col_Sheet_melt = 0;
    //int Col_Sheet_partno = 0;
    //int Col_Sheet_pack = 0;
    //int Col_Sheet_sheet = 0;
    //int Col_Sheet_temper = 0;
    //int Col_Sheet_speed = 0;
    //
    //int Col_Sheet_za_pt3 = 0;
    //int Col_Sheet_za_te3 = 0;
    //
    //int Col_Sheet_lampresstop = 0;
    //int Col_Sheet_lampressbot = 0;
    //int Col_Sheet_posclapantop = 0;
    //int Col_Sheet_posclapanbot = 0;
    //int Col_Sheet_mask = 0;
    //
    //int Col_Sheet_lam1posclapantop = 0;
    //int Col_Sheet_lam1posclapanbot = 0;
    //int Col_Sheet_lam2posclapantop = 0;
    //int Col_Sheet_lam2posclapanbot = 0;
    //
    //int Col_Sheet_lam_te1 = 0;
    //int Col_Sheet_news = 0;
    //int Col_Sheet_top1 = 0;
    //int Col_Sheet_top2 = 0;
    //int Col_Sheet_top3 = 0;
    //int Col_Sheet_top4 = 0;
    //int Col_Sheet_top5 = 0;
    //int Col_Sheet_top6 = 0;
    //int Col_Sheet_top7 = 0;
    //int Col_Sheet_top8 = 0;
    //
    //int Col_Sheet_bot1 = 0;
    //int Col_Sheet_bot2 = 0;
    //int Col_Sheet_bot3 = 0;
    //int Col_Sheet_bot4 = 0;
    //int Col_Sheet_bot5 = 0;
    //int Col_Sheet_bot6 = 0;
    //int Col_Sheet_bot7 = 0;
    //int Col_Sheet_bot8 = 0;
    //
    //int Col_Sheet_day = 0;
    //int Col_Sheet_month = 0;
    //int Col_Sheet_year = 0;
    //int Col_Sheet_cassetteno = 0;
    //int Col_Sheet_sheetincassette = 0;
    //int Col_Sheet_pos = 0;
    //
    //int Col_Sheet_timeforplateheat = 0;
    //int Col_Sheet_prestostartcomp = 0;
    //int Col_Sheet_slab = 0;
    //int Col_Sheet_subsheet = 0;
}

DLLRESULT FilterSheet()
{
    ListView_DeleteAllItems(ListSheet);
    //if(Col_Sheet_prestostartcomp == 0)
    //{
    //    std::string connand = "SELECT column_name, ordinal_position FROM information_schema.columns WHERE table_name = 'sheet' ORDER BY ordinal_position;";
    //    PGresult* res = conn_kpvl.PGexec(connand);
    //    SendDebug("conn_kpvl", connand);
    //    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    //    {
    //        int line = PQntuples(res);
    //        int nFields = PQnfields(res);
    //        for(int l = 0; l < line; l++)
    //        {
    //            std::string column = conn_kpvl.PGgetvalue(res, l, 0);
    //            int position = atoi(conn_kpvl.PGgetvalue(res, l, 1).c_str());
    //
    //            if("create_at" == column)Col_Sheet_create_at = position - 1;
    //            else if("zone" == column)Col_Sheet_zone = position - 1;
    //            else if("pos" == column)Col_Sheet_pos = position - 1;
    //            else if("id" == column)Col_Sheet_id = position - 1;
    //            else if("datatime_end" == column)Col_Sheet_datatime_end = position - 1;
    //            else if("datatime_all" == column)Col_Sheet_datatime_all = position - 1;
    //            else if("alloy" == column)Col_Sheet_alloy = position - 1;
    //            else if("thikness" == column)Col_Sheet_thikness = position - 1;
    //            else if("melt" == column)Col_Sheet_melt = position - 1;
    //            else if("slab" == column)Col_Sheet_slab = position - 1;
    //            else if("partno" == column)Col_Sheet_partno = position - 1;
    //            else if("pack" == column)Col_Sheet_pack = position - 1;
    //            else if("sheet" == column)Col_Sheet_sheet = position - 1;
    //            else if("subsheet" == column)Col_Sheet_subsheet = position - 1;
    //            else if("temper" == column)Col_Sheet_temper = position - 1;
    //            else if("speed" == column)Col_Sheet_speed = position - 1;
    //            else if("za_pt3" == column)Col_Sheet_za_pt3 = position - 1;
    //            else if("za_te3" == column)Col_Sheet_za_te3 = position - 1;
    //            else if("lampresstop" == column)Col_Sheet_lampresstop = position - 1;
    //            else if("lampressbot" == column)Col_Sheet_lampressbot = position - 1;
    //            else if("posclapantop" == column)Col_Sheet_posclapantop = position - 1;
    //            else if("posclapanbot" == column)Col_Sheet_posclapanbot = position - 1;
    //            else if("mask" == column)Col_Sheet_mask = position - 1;
    //            else if("lam1posclapantop" == column)Col_Sheet_lam1posclapantop = position - 1;
    //            else if("lam1posclapanbot" == column)Col_Sheet_lam1posclapanbot = position - 1;
    //            else if("lam2posclapantop" == column)Col_Sheet_lam2posclapantop = position - 1;
    //            else if("lam2posclapanbot" == column)Col_Sheet_lam2posclapanbot = position - 1;
    //            else if("lam_te1" == column)Col_Sheet_lam_te1 = position - 1;
    //            else if("news" == column)Col_Sheet_news = position - 1;
    //            else if("top1" == column)Col_Sheet_top1 = position - 1;
    //            else if("top2" == column)Col_Sheet_top2 = position - 1;
    //            else if("top3" == column)Col_Sheet_top3 = position - 1;
    //            else if("top4" == column)Col_Sheet_top4 = position - 1;
    //            else if("top5" == column)Col_Sheet_top5 = position - 1;
    //            else if("top6" == column)Col_Sheet_top6 = position - 1;
    //            else if("top7" == column)Col_Sheet_top7 = position - 1;
    //            else if("top8" == column)Col_Sheet_top8 = position - 1;
    //            else if("bot1" == column)Col_Sheet_bot1 = position - 1;
    //            else if("bot2" == column)Col_Sheet_bot2 = position - 1;
    //            else if("bot3" == column)Col_Sheet_bot3 = position - 1;
    //            else if("bot4" == column)Col_Sheet_bot4 = position - 1;
    //            else if("bot5" == column)Col_Sheet_bot5 = position - 1;
    //            else if("bot6" == column)Col_Sheet_bot6 = position - 1;
    //            else if("bot7" == column)Col_Sheet_bot7 = position - 1;
    //            else if("bot8" == column)Col_Sheet_bot8 = position - 1;
    //            else if("day" == column)Col_Sheet_day = position - 1;
    //            else if("month" == column)Col_Sheet_month = position - 1;
    //            else if("year" == column)Col_Sheet_year = position - 1;
    //            else if("cassetteno" == column)Col_Sheet_cassetteno = position - 1;
    //            else if("sheetincassette" == column)Col_Sheet_sheetincassette = position - 1;
    //            else if("start_at" == column)Col_Sheet_start_at = position - 1;
    //            else if("timeforplateheat" == column)Col_Sheet_timeforplateheat = position - 1;
    //            else if("prestostartcomp" == column)Col_Sheet_prestostartcomp = position - 1;
    //        }
    //    }
    //    else
    //    {
    //        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
    //        SendDebug("conn_kpvl", errc);
    //        SendDebug("conn_kpvl", connand);
    //    }
    //}

    //if(Col_Sheet_prestostartcomp == 0)
    //{
    //    std::string connand = "SELECT * FROM sheet WHERE id <= 1";
    //    PGresult* res = conn_kpvl.PGexec(connand);
    //    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    //    {
    //        int line = PQntuples(res);
    //        int nFields = PQnfields(res);
    //
    //        for(int position = 0; position < nFields; position++)
    //        {
    //            std::string column =  utf8_to_cp1251(PQfname(res, position));
    //
    //            //std::string column = conn_kpvl.PGgetvalue(res, l, 0);
    //            //int position = atoi(conn_kpvl.PGgetvalue(res, l, 1).c_str());
    //
    //            if("create_at" == column)Col_Sheet_create_at = position;
    //            else if("pos" == column)Col_Sheet_pos = position;
    //            else if("id" == column)Col_Sheet_id = position;
    //            else if("datatime_end" == column)Col_Sheet_datatime_end = position;
    //            else if("datatime_all" == column)Col_Sheet_datatime_all = position;
    //            else if("alloy" == column)Col_Sheet_alloy = position;
    //            else if("thikness" == column)Col_Sheet_thikness = position;
    //            else if("melt" == column)Col_Sheet_melt = position;
    //            else if("slab" == column)Col_Sheet_slab = position;
    //            else if("partno" == column)Col_Sheet_partno = position;
    //            else if("pack" == column)Col_Sheet_pack = position;
    //            else if("sheet" == column)Col_Sheet_sheet = position;
    //            else if("subsheet" == column)Col_Sheet_subsheet = position;
    //            else if("temper" == column)Col_Sheet_temper = position;
    //            else if("speed" == column)Col_Sheet_speed = position;
    //            else if("za_pt3" == column)Col_Sheet_za_pt3 = position;
    //            else if("za_te3" == column)Col_Sheet_za_te3 = position;
    //            else if("lampresstop" == column)Col_Sheet_lampresstop = position;
    //            else if("lampressbot" == column)Col_Sheet_lampressbot = position;
    //            else if("posclapantop" == column)Col_Sheet_posclapantop = position;
    //            else if("posclapanbot" == column)Col_Sheet_posclapanbot = position;
    //            else if("mask" == column)Col_Sheet_mask = position;
    //            else if("lam1posclapantop" == column)Col_Sheet_lam1posclapantop = position;
    //            else if("lam1posclapanbot" == column)Col_Sheet_lam1posclapanbot = position;
    //            else if("lam2posclapantop" == column)Col_Sheet_lam2posclapantop = position;
    //            else if("lam2posclapanbot" == column)Col_Sheet_lam2posclapanbot = position;
    //            else if("lam_te1" == column)Col_Sheet_lam_te1 = position;
    //            else if("news" == column)Col_Sheet_news = position;
    //            else if("top1" == column)Col_Sheet_top1 = position;
    //            else if("top2" == column)Col_Sheet_top2 = position;
    //            else if("top3" == column)Col_Sheet_top3 = position;
    //            else if("top4" == column)Col_Sheet_top4 = position;
    //            else if("top5" == column)Col_Sheet_top5 = position;
    //            else if("top6" == column)Col_Sheet_top6 = position;
    //            else if("top7" == column)Col_Sheet_top7 = position;
    //            else if("top8" == column)Col_Sheet_top8 = position;
    //            else if("bot1" == column)Col_Sheet_bot1 = position;
    //            else if("bot2" == column)Col_Sheet_bot2 = position;
    //            else if("bot3" == column)Col_Sheet_bot3 = position;
    //            else if("bot4" == column)Col_Sheet_bot4 = position;
    //            else if("bot5" == column)Col_Sheet_bot5 = position;
    //            else if("bot6" == column)Col_Sheet_bot6 = position;
    //            else if("bot7" == column)Col_Sheet_bot7 = position;
    //            else if("bot8" == column)Col_Sheet_bot8 = position;
    //            else if("day" == column)Col_Sheet_day = position;
    //            else if("month" == column)Col_Sheet_month = position;
    //            else if("year" == column)Col_Sheet_year = position;
    //            else if("cassetteno" == column)Col_Sheet_cassetteno = position;
    //            else if("sheetincassette" == column)Col_Sheet_sheetincassette = position;
    //            else if("start_at" == column)Col_Sheet_start_at = position;
    //            else if("timeforplateheat" == column)Col_Sheet_timeforplateheat = position;
    //            else if("prestostartcomp" == column)Col_Sheet_prestostartcomp = position;
    //        }
    //    }
    //    else
    //    {
    //        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
    //        SendDebug("conn_kpvl", errc);
    //        SendDebug("conn_kpvl", FilterComand);
    //    }
    //    PQclear(res);
    //}
return 0;
}

std::string strYear;
std::string strMonth;
std::string strDay;
std::string strCassetteNo = "1";

BOOL bFilterData = TRUE;

std::string FindTag(std::string name)
{
    for(auto t : AllTag)
    {
        if(t.name == name) return t.content;
    }
    return "";
}

void FilterIDCasseteSheet(int stryear, int strmonth, int strday, int strcassetteno)
{
    strYear = std::to_string(stryear);
    strMonth = std::to_string(strmonth);
    strDay = std::to_string(strday);
    strCassetteNo = std::to_string(strcassetteno);

    FilterComand = "SELECT * FROM sheet ";
    FilterComand += "WHERE ";
    FilterComand += "year = '" + strYear + "' AND ";
    FilterComand += "month = '" + strMonth + "' AND ";
    FilterComand += "day = '" + strDay + "' AND ";
    FilterComand += "cassetteno = '" + strCassetteNo + "' ";
    FilterComand += "ORDER BY create_at DESC";
    FilterComand += ";";
    FilterSheet();
    bFilterData = FALSE;
}

void FilterDataTimeSheet()
{
    ListView_DeleteAllItems(ListSheet);
    AllSheet.erase(AllSheet.begin(), AllSheet.end());

    FilterComand = "SELECT * FROM sheet ";
    FilterComand += "WHERE create_at >= ";
    FilterComand += "'" + DataStartSheet + "'";
    FilterComand += " AND create_at <= ";
    FilterComand += "'" + DataStopSheet + " 23:59:59.999' ";
    FilterComand += "ORDER BY create_at DESC";
    FilterSheet();
    bFilterData = TRUE;
}

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
        HDITEM hi = { 0 };
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

std::map<int, const char*> NameZone ={
    {0, "На входе" },
    {1, "1-я часть печи"},
    {2, "2-я часть печи"},
    {3, "Закалка"},
    {4, "Охлаждение"},
    {5, "Выдача"},
    {6, "Кантовка"},
    {7, "В касете"},
};



DLLRESULT CommandPassportSheetDialog(HWND hWnd, WPARAM wParam)
{
    if(wParam == IDOK)
    {
        if(SavePasportExel(hWnd))
            EndDialog(hWnd, FALSE);
    }
    else if(wParam == IDCANCEL)
    {
        EndDialog(hWnd, FALSE);
    }
    return 0;
}

void GetCassete(TSheet& p, TCassette& сassette)
{
    strYear = std::to_string(atoi(p.Year.c_str()));
    strMonth = std::to_string(atoi(p.Month.c_str()));
    strDay = std::to_string(atoi(p.Day.c_str()));
    strCassetteNo = std::to_string(atoi(p.CassetteNo.c_str()));

    std::string FilterComand1 = "SELECT * FROM cassette ";
    FilterComand1 += "WHERE ";
    FilterComand1 += "year = '" + strYear + "' AND ";
    FilterComand1 += "month = '" + strMonth + "' AND ";
    FilterComand1 += "day = '" + strDay + "' AND ";
    FilterComand1 += "cassetteno = '" + strCassetteNo + "' ";
    FilterComand1 += "ORDER BY create_at DESC";
    FilterComand1 += ";";

    PGresult* res = conn_kpvl.PGexec(FilterComand1);
    SendDebug("conn_kpvl", FilterComand1);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        int nFields = PQnfields(res);
        for(int l = 0; l < line; l++)
        {
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

        }
    }
    else
    {
        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        SendDebug("conn_kpvl", errc);
        SendDebug("conn_kpvl", FilterComand1);
    }
    PQclear(res);

}

////Отрисовка текста
//LRESULT DrawGraffStat(HWND hWnd, PAINTSTRUCT ps, int TT)
//{
//    char szBuff[MAX_STRING] = "";
//    GetWindowText(hWnd, szBuff, 255);
//    if(strlen(szBuff))
//    {
//        SIZE siz;
//        GetTextExtentPoint32(ps.hdc, szBuff, (int)strlen(szBuff), &siz);
//        if(siz.cx < ps.rcPaint.right - ps.rcPaint.left)
//        {
//            char* s = szBuff;
//            int coco = 1;
//            while(s && *s) { if(*(s++) == '\n')coco++; }
//            coco *= siz.cy / 2;
//
//            int y = (ps.rcPaint.bottom - ps.rcPaint.top) / 2;
//            ps.rcPaint.top = y - coco;
//            ps.rcPaint.bottom = y + coco;
//            ps.rcPaint.left++;
//            ps.rcPaint.right--;
//            //DrawTextA(pDIS->hDC, szBuff, -1, &pDIS->rcItem, TT | DT_WORDBREAK | DT_VCENTER);
//        }
//        SetBkMode(ps.hdc, TRANSPARENT);
//        DrawTextA(ps.hdc, szBuff, -1, &ps.rcPaint, TT | DT_WORDBREAK | DT_VCENTER);
//    }
//    return 0;
//}
//
//std::deque<double> test;
//
////Отрисовка статичного окна
//LRESULT DrawStatic(LPDRAWITEMSTRUCT pDIS)
//{
//    HBRUSH* br = (HBRUSH*)GetWindowLongPtr(pDIS->hwndItem, GWLP_USERDATA);
//    HDC hdc = pDIS->hDC;
//    if(br)
//        FillRect(pDIS->hDC, &pDIS->rcItem, *br);
//    else
//        FillRect(pDIS->hDC, &pDIS->rcItem, TitleBrush3);
//
//    DWORD style = GetWindowStyle(pDIS->hwndItem);
//
//    //int size = DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_2.ToHmi.TAct"].size();
//    //if(size)
//    //{
//        //DeleteFile("Test.csv");
//        //LRESULT times = DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_2.ToHmi.TAct"].back().f;
//        //for(auto D : DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_2.ToHmi.TAct"])
//        //{
//        //    std::ofstream F("Test.csv", std::ios::binary | std::ios::out | std::ios::app | std::ios::ate);
//        //    if(F.is_open())
//        //    {
//        //        F << "'";
//        //        F << D.d;
//        //        F << ";";
//        //        F << D.f;
//        //        F << "\n";
//        //        F.close();
//        //    }
//        //}
//    //}
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_2.ToHmi.TAct"];
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_3.ToHmi.TAct"];
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_4.ToHmi.TAct"];
//    //
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_1.ToHmi.TAct"];
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_1.ToHmi.TAct"];
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_2.ToHmi.TAct"];
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_3.ToHmi.TAct"];
//    //DataTemp["|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_4.ToHmi.TAct"];
//    //
//    //if(DataTempSet1.size())
//    //{
//    //    float maxTemp = -1000;
//    //    float minTemp = 0;
//    //    for(auto x : DataTempSet1)
//    //    {
//    //        maxTemp = max(x, maxTemp);
//    //    }
//    //
//    //
//    //    float yCountTemp = DataTempSet1.size();
//    //    float yCountPoint = pDIS->rcItem.right - pDIS->rcItem.left;
//    //
//    //    float yK = yCountPoint / yCountTemp - 10;
//    //
//    //    float xCountTemp = maxTemp;
//    //    float xCountPoint = pDIS->rcItem.bottom - pDIS->rcItem.top - 10;
//    //
//    //    float xK = xCountPoint / xCountTemp;
//    //
//    //    test.erase(test.begin(), test.end());
//    //    MoveToEx(pDIS->hDC, 0, pDIS->rcItem.bottom - pDIS->rcItem.top, NULL);
//    //    for(int i = 0; i < yCountPoint; i++)
//    //    {
//    //        double ii;
//    //        int t = modf(i / yK, &ii);
//    //
//    //        float df = DataTempSet1[t];
//    //        float ff = (df * xK) + 5.0;
//    //        test.push_back(ff);
//    //        LineTo(pDIS->hDC, ff, i + 5);
//    //    }
//    //}
//    //
//    //
//    //LineTo(hdc, 3, 190);
//    //
//    //
//    //int Al = DT_CENTER;
//    //if((style & MYSS_LEFT) == MYSS_LEFT)Al = DT_LEFT;
//    //if((style & MYSS_RIGHT) == MYSS_RIGHT)Al = DT_RIGHT;
//    //SetTextColor(pDIS->hDC, RGB(0, 0, 0));
//    //DrawTextStat(pDIS, Al);
//
//    return 0;
//}
//
////Проверка стиля для отрисовки окна
//LRESULT DrawItem(LPDRAWITEMSTRUCT pDIS)
//{
//    char szBuff[MAX_STRING] = "";
//    GetClassName(pDIS->hwndItem, szBuff, MAX_STRING);
//    CharUpper(szBuff);
//    if(boost::iequals(szStat, szBuff))
//        return DrawStatic(pDIS);
//
//    return 0;
//}



LRESULT GetTM(std::string datatime, std::tm& TM)
{
    //boost::replace_all(datatime, "+", "\0");

    std::vector <std::string>split;
    boost::split(split, datatime, boost::is_any_of(" "), boost::token_compress_on);
    if(split.size() == 2)
    {
        std::vector <std::string>data;
        boost::split(data, split[0], boost::is_any_of("-"), boost::token_compress_on);
        if(data.size() == 3)
        {
            std::vector <std::string>times;
            boost::split(times, split[1], boost::is_any_of(":"), boost::token_compress_on);
            if(times.size() == 3)
            {
                time_t st = time(0);
                std::tm TM;
                localtime_s(&TM, &st);

                TM.tm_year = atoi(data[0].c_str()) - 1900;
                TM.tm_mon = atoi(data[1].c_str()) - 1;
                TM.tm_mday = atoi(data[1].c_str());
                TM.tm_hour = atoi(times[0].c_str());
                TM.tm_min = atoi(times[1].c_str());
                TM.tm_sec = atoi(times[2].c_str());
                LRESULT r = (LRESULT)mktime(&TM);
                return r;
            }
        }
    }
    return 0;
}

float GetDataTempSheet(TSheet& p, std::string name, int pos)
{
    //std::string command = "SELECT content, content_at FROM TAG WHERE name = '" + name + "';";
    //float first = 0;
    //PGresult* res = conn_kpvl.PGexec(command);
    //SendDebug("conn_kpvl", command);
    //if(PQresultStatus(res) == PGRES_TUPLES_OK)
    //{
    //    int line = PQntuples(res);
    //    if(line)
    //    {
    //        int nFields = PQnfields(res);
    //        std::string F = conn_kpvl.PGgetvalue(res, 0, 0);
    //        first = (float)atof(F.c_str());
    //    }
    //}
    float temp = 0;
    std::string command = "SELECT create_at, content FROM todos WHERE id_name = (SELECT id_name FROM tag WHERE name = '" + name + "') AND";
    command += " create_at >= '" + p.Start_at + "' AND";
    command += " create_at <= '" + p.DataTime_End + "'";
    command += " ORDER BY create_at;";
    PGresult* res = conn_kpvl.PGexec(command);
    SendDebug("conn_kpvl", command);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        
        if(line)
        {
            int nFields = PQnfields(res);
            LRESULT TT = 0;
            for(int l = 0; l < line; l++)
            {
                std::string datatime = conn_kpvl.PGgetvalue(res, l, 0);
                auto w = boost::find_first(datatime, "+");
                if(!w.empty())
                    w.front() = '\0';

                std::tm TM;
                LRESULT TR = GetTM(datatime, TM);
                if(!TT) TT = TR;
                temp = (float)atof(conn_kpvl.PGgetvalue(res, l, 1).c_str());
                qwe q ={name, pos, temp};
                DataTempSheet[datatime].push_back(q);
            }
            PQclear(res);
            return temp;
        }
    }
    else
    {
        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        SendDebug("conn_kpvl", errc);
        SendDebug("conn_kpvl", command);
        PQclear(res);
    }
    return temp;
}


void GetTemperSheet(HWND hWnd, LPARAM lParam)
{
    if(lParam >= 0 && lParam < (int)AllSheet.size())
    {
        TSheet& p = AllSheet[lParam];
        {
            SetWindowText(GetDlgItem(hWnd, IDC_pAlloy), p.Alloy.c_str());           //Марка стали
            SetWindowText(GetDlgItem(hWnd, IDC_pThikness), p.Thikness.c_str());     //Толщина листа
            SetWindowText(GetDlgItem(hWnd, IDC_pMelt), p.Melt.c_str());             //Плавка
            SetWindowText(GetDlgItem(hWnd, IDC_pPartNo), p.PartNo.c_str());         //Партия
            SetWindowText(GetDlgItem(hWnd, IDC_pPack), p.Pack.c_str());             //Пачка
            SetWindowText(GetDlgItem(hWnd, IDC_pSheet), p.Sheet.c_str());           //Лист
            SetWindowText(GetDlgItem(hWnd, IDC_pSlab), "0");                        //Сляб

            std::vector <std::string>split;
            std::string DataTime = p.Start_at;
            boost::split(split, DataTime, boost::is_any_of(" "), boost::token_compress_on);
            if(split.size() == 2)
            {
                SetWindowText(GetDlgItem(hWnd, IDC_pSheetDate), split[0].c_str());  //Дата
                SetWindowText(GetDlgItem(hWnd, IDC_pSheetTime), split[1].c_str());  //Время
            }
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT11), p.TimeForPlateHeat.c_str());//Задание Время нахождения листа в закалочной печи. мин
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT12), p.DataTime_All.c_str());    //Факт Время нахождения листа в закалочной печи. мин

            SetWindowText(GetDlgItem(hWnd, IDC_EDIT13), p.Temper.c_str());          //Задание Температуры закалки
            

            SetWindowText(GetDlgItem(hWnd, IDC_EDIT15), p.PresToStartComp.c_str()); //Задание Давления воды
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT16), p.Za_PT3.c_str());          //Факт Давления воды

            SetWindowText(GetDlgItem(hWnd, IDC_EDIT17), p.Za_TE3.c_str());         //Факт Температура воды, °С
        }
        DataTempSheet.erase(DataTempSheet.begin(), DataTempSheet.end());
        if(p.Start_at.length()/* && p.DataTime_End.length()*/)
        {
            int line = 0;
            //GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.GenSeqFromHmi.Data.TempSet1", 2); //Задание Температуры закалки
                                                  //Application.GenSeqFromHmi.Data.TempSet1

            GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.AI_Hmi_210.Hmi.Za_TE4.AI_eu", 3);

            GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_1.ToHmi.TAct", 4);
            GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_2.ToHmi.TAct", 5);
            GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_3.ToHmi.TAct", 6);
            GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_4.ToHmi.TAct", 7);

            float t21 = GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_1.ToHmi.TAct", 8);
            float t22 = GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_2.ToHmi.TAct", 9);
            float t23 = GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_3.ToHmi.TAct", 10);
            float t24 = GetDataTempSheet(p, "|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_4.ToHmi.TAct", 11);
            float t = (float)((t21 + t22 + t23 + t24) / 4.0);
            char ss[256];
            sprintf(ss, "%0.1f", t);
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT14), ss);                        //Факт Температуры закалки

            #define COUNT_COLUMN_SHEET 12
            int iItemServ = 0;
            //std::vector<float>first[COUNT_COLUMN_SHEET] = {
            float first[COUNT_COLUMN_SHEET]{
                0, 0,
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.GenSeqFromHmi.Data.TempSet1").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.AI_Hmi_210.Hmi.Za_TE4.AI_eu").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_1.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_2.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_3.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr_4.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_1.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_2.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_3.ToHmi.TAct").c_str()),
                (float)atof(FindTag("|var|PLC210 OPC-UA.Application.Hmi210_1.Htr2_4.ToHmi.TAct").c_str()),
            };

            //int iItemServ = 0;
            for(auto& d : DataTempSheet)
            {
                char ss[256];
                sprintf_s(ss, 256, "%d", iItemServ + 1);

                LV_ITEM lvi ={0};
                lvi.mask = LVIF_TEXT;
                lvi.pszText = (LPSTR)ss;
                lvi.cchTextMax = 255;
                lvi.iItem = (int)iItemServ;
                lvi.lParam = NULL;
                LRESULT ret = ListView_InsertItem(ListSheetTemp, &lvi);

                ListView_SetItemText(ListSheetTemp, lvi.iItem, 1, (LPSTR)d.first.c_str());
                for(int i = 2; i < COUNT_COLUMN_SHEET; i++)
                {
                    BOOL end = FALSE;
                    for(auto& s : d.second)
                    {
                        if(s.t == i)
                        {
                            char ss[256];
                            sprintf_s(ss, 256, "%.1f", s.f);
                            ListView_SetItemText(ListSheetTemp, lvi.iItem, i, (LPSTR)ss);
                            first[i] = s.f;
                            end = TRUE;
                            break;
                        }
                    }
                    if(!end)
                    {
                        char ss[256];
                        sprintf_s(ss, 256, "%.1f", first[i]);
                        ListView_SetItemText(ListSheetTemp, lvi.iItem, i, (LPSTR)ss);
                    }
                }
                iItemServ++;
            }
        }
    }
}




void GetDataTempCassete(TCassette& p, std::string name, int pos)
{
    std::string command = "SELECT create_at, content FROM todos WHERE id_name = (SELECT id_name FROM tag WHERE name = '" + name + "') AND";
    command += " create_at >= '" + p.Run_at + "'";
    if(p.End_at.length())command += " AND create_at <= '" + p.End_at + "'";
    command += " ORDER BY create_at;";
    PGresult* res = conn_kpvl.PGexec(command);
    SendDebug("conn_kpvl", command);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        if(line)
        {
            int nFields = PQnfields(res);
            LRESULT TT = 0;
            for(int l = 0; l < line; l++)
            {
                std::string datatime = conn_kpvl.PGgetvalue(res, l, 0);
                auto w = boost::find_first(datatime, "+");
                if(!w.empty())
                    w.front() = '\0';

                std::tm TM;
                LRESULT TR = GetTM(datatime, TM);
                if(!TT) TT = TR;
                qwe q ={name, pos, (float)atof(conn_kpvl.PGgetvalue(res, l, 1).c_str())};
                DataTempCassette[datatime].push_back(q);
            }
            PQclear(res);
        }
    }
    else
    {
        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        SendDebug("conn_kpvl", errc);
        SendDebug("conn_kpvl", command);
        PQclear(res);
    }
}

void GetTemperCassette(HWND hWnd, LPARAM lParam)
{
    if(lParam >= 0 && lParam < (int)AllSheet.size())
    {
        TSheet& p = AllSheet[lParam];
        {
            TCassette сassette;
            GetCassete(p, сassette);

            //GetTM(std::string datatime, std::tm & TM)
            std::vector <std::string>split;
            std::string DataTime = сassette.Run_at;
            boost::split(split, DataTime, boost::is_any_of(" "), boost::token_compress_on);
            if(split.size() == 2)
            {
                SetWindowText(GetDlgItem(hWnd, IDC_pCassetteDate), split[0].c_str());
                SetWindowText(GetDlgItem(hWnd, IDC_pCasseteTime), split[1].c_str());
            }

            //Время нагрева до температуры отпуска, мин
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT18), сassette.PointTime_1.c_str());  //Задание
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT19), "");                            //Факт

            //Время выдержки при заданной температуре, мин
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT20), сassette.PointDTime_2.c_str()); //Задание
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT21), "");                            //Факт

            //Температура отпуска, °С
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT22), сassette.PointRef_1.c_str());   //Задание
            SetWindowText(GetDlgItem(hWnd, IDC_EDIT23), сassette.f_temper.c_str());                 //Факт. За 5 минут лдо конца отпуска

            float first[6] ={0, 0,0, 0,0, 0};
            DataTempCassette.erase(DataTempCassette.begin(), DataTempCassette.end());
            int iItemServ = 0;
            if(сassette.Run_at.length())
            {
                if(atoi(сassette.Peth.c_str()) == 1)
                {
                    float t1 = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.ProcTimeMin").c_str());      //REAL Время до окончания процесса, мин TimeToProcEnd
                    
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.TempRef", 2);    //Заданное значение температуры
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.TempAct", 3);    //Фактическое значение температуры
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.T1", 4);         //Термопара 1
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.T2", 5);         //Термопара 2

                    first[2] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.TempRef").c_str());
                    first[3] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.TempAct").c_str());
                    first[4] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.T1").c_str());
                    first[5] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.T2").c_str());
                }
                if(atoi(сassette.Peth.c_str()) == 2)
                {
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.TempRef", 2);    //Заданное значение температуры
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.TempAct", 3);    //Фактическое значение температуры
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.T1", 4);         //Термопара 1
                    GetDataTempCassete(сassette, "|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.T2", 5);         //Термопара 2

                    first[2] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.TempRef").c_str());
                    first[3] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.TempAct").c_str());
                    first[4] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.T1").c_str());
                    first[5] = (float)atof(FindTag("|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.T2").c_str());
                }

                for(auto& d : DataTempCassette)
                {
                    char ss[256];
                    sprintf_s(ss, 256, "%d", iItemServ + 1);

                    LV_ITEM lvi ={0};
                    lvi.mask = LVIF_TEXT;
                    lvi.pszText = (LPSTR)ss;
                    lvi.cchTextMax = 255;
                    lvi.iItem = (int)iItemServ;
                    lvi.lParam = NULL;// param;
                    LRESULT ret = ListView_InsertItem(ListCasseteTemp, &lvi);

                    ListView_SetItemText(ListCasseteTemp, lvi.iItem, 1, (LPSTR)d.first.c_str());
                    for(int i = 2; i < 6; i++)
                    {
                        BOOL end = FALSE;
                        for(auto& s : d.second)
                        {
                            if(s.t == i)
                            {
                                char ss[256];
                                sprintf_s(ss, 256, "%.1f", s.f);
                                ListView_SetItemText(ListCasseteTemp, lvi.iItem, i, (LPSTR)ss);
                                first[i] = s.f;
                                end = TRUE;
                                break;
                            }
                        }
                        if(!end)
                        {
                            char ss[256];
                            sprintf_s(ss, 256, "%.1f", first[i]);
                            ListView_SetItemText(ListCasseteTemp, lvi.iItem, i, (LPSTR)ss);
                        }

                    }
                    iItemServ++;
                }
            }
        }
    }
}

//BOOL CenterWindow2(HWND hwndChild, HWND hwndParent);
DLLRESULT InitPassportSheetDialog(HWND hDlg, LPARAM lParam)
{
    #define Stat03Flag WS_CHILD | WS_VISIBLE | WS_BORDER | SS_OWNERDRAW | SS_CENTERIMAGE | DT_SINGLELINE

    #define Flag WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER

    ListSheetTemp = CreateWindowEx(0, "SysListView32", "", Flag,   17, 125, 680, 195, hDlg, (HMENU)hEdit_Sheet, hInstance, nullptr);
    ListCasseteTemp = CreateWindowEx(0, "SysListView32", "", Flag, 17, 350, 680, 165, hDlg, (HMENU)hEdit_Sheet, hInstance, nullptr);

    //ListSheetTemp = GetDlgItem(hDlg, IDC_LIST2);
    //SetWindowLongPtr(ListSheetTemp, GWL_STYLE, Flag);
    //SetWindowPos()
    if(!ListSheetTemp)
        throw std::exception(std::string("Ошибка создания окна : ListSheetTemp").c_str());

    HWND hHeaderSheetTemp = ListView_GetHeader(ListSheetTemp);
    SetWindowSubclass(hHeaderSheetTemp, WndProcHeadListViewSheet, 1, 32);

    SendMessage(ListSheetTemp, WM_SETFONT, (WPARAM)Font[emFont::Font09], FALSE);
    ListView_SetExtendedListViewStyle(ListSheetTemp, LVS_EX_HEADERINALLVIEWS | LVS_EX_DOUBLEBUFFER | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_REGIONAL | LVS_EX_FULLROWSELECT);

    for(auto a : SheetTemp_Collumn | boost::adaptors::indexed(0))
        AddColumn(ListSheetTemp, a.index(), &a.value().second);

    //HWND ParentSheet = GetDlgItem(hWnd, IDC_STATIC1);
    //SetWindowPos(ListSheetTemp, HWND_TOP, 0, 0, 680, 180, 0);
    //CenterWindow2(ListSheetTemp, ParentSheet);
    //MoveWindow(ListSheetTemp, 20, 125, 680, 180, true);

    GetTemperSheet(hDlg, lParam);

    //ListCasseteTemp = GetDlgItem(hDlg, IDC_LIST3);
    //SetWindowLongPtr(ListCasseteTemp, GWL_STYLE, Flag);

    if(!ListCasseteTemp)
        throw std::exception(std::string("Ошибка создания окна : ListCasseteTemp").c_str());

    HWND hHeaderCasseteTemp = ListView_GetHeader(ListCasseteTemp);
    SetWindowSubclass(hHeaderCasseteTemp, WndProcHeadListViewSheet, 1, 32);

    SendMessage(ListCasseteTemp, WM_SETFONT, (WPARAM)Font[emFont::Font09], FALSE);
    ListView_SetExtendedListViewStyle(ListCasseteTemp, LVS_EX_HEADERINALLVIEWS | LVS_EX_DOUBLEBUFFER | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_REGIONAL | LVS_EX_FULLROWSELECT);

    for(auto a : CasseteTemp_Collumn | boost::adaptors::indexed(0))
        AddColumn(ListCasseteTemp, a.index(), &a.value().second);

    //HWND ParentCassete = GetDlgItem(hWnd, IDC_STATIC2);
    //SetWindowPos(ListCasseteTemp, HWND_TOP, 0, 0, 680, 140, 0);
    //CenterWindow2(ListCasseteTemp, ParentCassete);
    //MoveWindow(ListCasseteTemp, 20, 345, 680, 140, true);

    GetTemperCassette(hDlg, lParam);

    HWND child = GetWindow(hDlg, GW_CHILD);
    do
    {
        SendMessage(child, WM_SETFONT, (WPARAM)Font[emFont::Font11], TRUE);
    } while(child = GetWindow(child, GW_HWNDNEXT));

    return 0;
}


DLLRESULT CALLBACK SheetPasportProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_INITDIALOG) return InitPassportSheetDialog(hDlg, lParam);
    if(message == WM_COMMAND) return CommandPassportSheetDialog(hDlg, wParam);
    //if(message == WM_DRAWITEM) return DrawItem((LPDRAWITEMSTRUCT)lParam);
    return 0;
}


LRESULT OnNotifySheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNM_LISTVIEW  pnm = (LPNM_LISTVIEW)lParam;
    if(pnm->hdr.code == NM_DBLCLK)
    {
        LPNMITEMACTIVATE lvi = (LPNMITEMACTIVATE)lParam;
        if(lvi->iItem >= 0 && lvi->iItem < (int)AllSheet.size())
        {
            TSheet& p = AllSheet[lvi->iItem];
            DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, SheetPasportProc, lvi->iItem);
            //FilterIDCasseteSheet(atoi(p.Year.c_str()), atoi(p.Month.c_str()), atoi(p.Day.c_str()), atoi(p.CassetteNo.c_str()));
        }
    }
    else if(pnm->hdr.code == LVN_GETDISPINFO)
    {
        LV_DISPINFO* plvdi = (LV_DISPINFO*)lParam;

        int item = plvdi->item.iItem;
        int subItem = plvdi->item.iSubItem;
        if(item >= 0 && item < (int)AllSheet.size())
        {
            if(plvdi->item.mask & LVIF_TEXT)
            {
                TSheet& p = AllSheet[item];
                {
                    if(subItem == casSheet::NN)         lstrcpy(plvdi->item.pszText, std::to_string(item + 1).c_str());
                    ELSEIF (casSheet::ID, p.id.c_str());
                    ELSEIF (casSheet::DataTime, p.DataTime.c_str());
                    ELSEIF (casSheet::Start_at, p.Start_at.c_str());
                    ELSEIF (casSheet::DataTime_End, p.DataTime_End.c_str());
                    ELSEIF (casSheet::DataTime_All, p.DataTime_All.c_str());

                    //ELSEIF (casSheet::Zone,             NameZone[atoi(p.Zone.c_str())]);
                    ELSEIF (casSheet::Pos, NameZone[atoi(p.Pos.c_str())]);

                    ELSEIF (casSheet::Alloy, p.Alloy.c_str());
                    ELSEIF (casSheet::Thikness, p.Thikness.c_str());

                    ELSEIF (casSheet::Melt, p.Melt.c_str());
                    ELSEIF (casSheet::Slab, p.Slab.c_str());
                    ELSEIF (casSheet::PartNo, p.PartNo.c_str());
                    ELSEIF (casSheet::Pack, p.Pack.c_str());
                    ELSEIF (casSheet::Sheet, p.Sheet.c_str());
                    ELSEIF (casSheet::SubSheet, p.SubSheet.c_str());
                    ELSEIF (casSheet::Temper, p.Temper.c_str());
                    ELSEIF (casSheet::Speed, p.Speed.c_str());
                    ELSEIF (casSheet::Za_PT3, p.Za_PT3.c_str());
                    ELSEIF (casSheet::Za_TE3, p.Za_TE3.c_str());

                    ELSEIF (casSheet::LamPressTop, p.LaminPressTop.c_str());
                    ELSEIF (casSheet::LamPressBot, p.LaminPressBot.c_str());

                    ELSEIF (casSheet::PosClapanTop, p.PosClapanTop.c_str());
                    ELSEIF (casSheet::PosClapanBot, p.PosClapanBot.c_str());
                    ELSEIF (casSheet::Mask, p.Mask.c_str());
                    ELSEIF (casSheet::Lam1PosClapanTop, p.Lam1PosClapanTop.c_str());
                    ELSEIF (casSheet::Lam1PosClapanBot, p.Lam1PosClapanBot.c_str());
                    ELSEIF (casSheet::Lam2PosClapanTop, p.Lam2PosClapanTop.c_str());
                    ELSEIF (casSheet::Lam2PosClapanBot, p.Lam2PosClapanBot.c_str());

                    ELSEIF (casSheet::Lam_TE1, p.LAM_TE1.c_str());


                    ELSEIF (casSheet::News, p.News.c_str());

                    ELSEIF (casSheet::Top1, p.Top1.c_str());
                    ELSEIF (casSheet::Top2, p.Top2.c_str());
                    ELSEIF (casSheet::Top3, p.Top3.c_str());
                    ELSEIF (casSheet::Top4, p.Top4.c_str());
                    ELSEIF (casSheet::Top5, p.Top5.c_str());
                    ELSEIF (casSheet::Top6, p.Top6.c_str());
                    ELSEIF (casSheet::Top7, p.Top7.c_str());
                    ELSEIF (casSheet::Top8, p.Top8.c_str());

                    ELSEIF (casSheet::Bot1, p.Bot1.c_str());
                    ELSEIF (casSheet::Bot2, p.Bot2.c_str());
                    ELSEIF (casSheet::Bot3, p.Bot3.c_str());
                    ELSEIF (casSheet::Bot4, p.Bot4.c_str());
                    ELSEIF (casSheet::Bot5, p.Bot5.c_str());
                    ELSEIF (casSheet::Bot6, p.Bot6.c_str());
                    ELSEIF (casSheet::Bot7, p.Bot7.c_str());
                    ELSEIF (casSheet::Bot8, p.Bot8.c_str());

                    ELSEIF (casSheet::Day, p.Day.c_str());
                    ELSEIF (casSheet::Month, p.Month.c_str());
                    ELSEIF (casSheet::Year, p.Year.c_str());
                    ELSEIF (casSheet::CassetteNo, p.CassetteNo.c_str());
                    ELSEIF (casSheet::SheetInCassette, p.SheetInCassette.c_str());
                    ELSEIF (casSheet::TimeForPlateHeat, p.TimeForPlateHeat.c_str()); //Время сигнализации окончания нагрева, мин
                    ELSEIF (casSheet::PresToStartComp, p.PresToStartComp.c_str());  //Уставка давления для запуска комперссора
                    //ELSEIF (casSheet::TempWatTankCool, p.TempWatTankCool.c_str());  //Температура закалочной воды для вкл.охлаждения

                }
            }
        }
    }
    return 0;
}

DLLRESULT InitFilterDataSheetDialog(HWND hWnd)
{
    CenterWindow(hWnd, SheetWindow);
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), DataStartSheet.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), DataStopSheet.c_str());
    return 0;
}

DLLRESULT InitFilterIDSheetDialog(HWND hWnd)
{
    CenterWindow(hWnd, SheetWindow);
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), strYear.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), strMonth.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT4), strDay.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT6), strCassetteNo.c_str());
    return 0;
}

DLLRESULT CommandFilterDataSheetDialog(HWND hWnd, WPARAM wParam)
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
            DataStartSheet = ss;
        else
            return MessageBox(hWnd, (std::string(errordata) + ss).c_str(), "Ошибка", 0);

        GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), ss, 255);
        if(TestData(ss))
            DataStopSheet = ss;
        else
            return MessageBox(hWnd, (std::string(errordata) + ss).c_str(), "Ошибка", 0);

        FilterDataTimeSheet();

        EndDialog(hWnd, FALSE);
    }
    else if(wParam == IDCANCEL)
    {
        EndDialog(hWnd, FALSE);
    }
    return 0;
}

DLLRESULT CommandFilterIDSheetDialog(HWND hWnd, WPARAM wParam)
{
    if(wParam == IDC_BUTTON1)
    {
        CreateCalendar(hWnd, 1);
    }
    else if(wParam == IDOK)
    {
        std::string datatest;
        char ssYear[255];
        char ssMonth[255];
        char ssDay[255];
        char ssCassetteNo[255];
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT1), ssYear, 255);
        datatest = std::string(ssYear) + "-";
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), ssMonth, 255);
        datatest += std::string(ssMonth) + "-";
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT4), ssDay, 255);
        datatest += std::string(ssDay) + "";

        GetWindowText(GetDlgItem(hWnd, IDC_EDIT6), ssCassetteNo, 255);
        if(TestData(datatest))
        {
            strYear = ssYear;
            strMonth = ssMonth;
            strDay = ssDay;
            //DataStartSheet = ss;
        }
        else
            return MessageBox(hWnd, (std::string(errordata) + " " + datatest).c_str(), "Ошибка", 0);


        FilterIDCasseteSheet(atoi(ssYear), atoi(ssMonth), atoi(ssDay), atoi(ssCassetteNo));

        EndDialog(hWnd, FALSE);
    }
    else if(wParam == IDCANCEL)
    {
        EndDialog(hWnd, FALSE);
    }
    return 0;
}


DLLRESULT UserDataSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(wParam == 1)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];
        sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM->wYear, TM->wMonth, TM->wDay);
        DataStartSheet = sFormat;
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), DataStartSheet.c_str());
    }
    if(wParam == 2)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];
        sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM->wYear, TM->wMonth, TM->wDay);
        DataStopSheet = sFormat;

        SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), DataStopSheet.c_str());
    }
    return 0;
}


DLLRESULT UserIDSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(wParam == 1)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];

        sprintf_s(sFormat, 50, "%4d", TM->wYear);
        strYear = sFormat;

        sprintf_s(sFormat, 50, "%2d", TM->wMonth);
        strMonth = sFormat;

        sprintf_s(sFormat, 50, "%2d", TM->wDay);
        strDay = sFormat;

        SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), strYear.c_str());
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), strMonth.c_str());
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT4), strDay.c_str());
    }

    return 0;
}

DLLRESULT CALLBACK FilterSheetIDProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER)return UserIDSheet(hWnd, message, wParam, lParam);
    if(message == WM_INITDIALOG)return InitFilterIDSheetDialog(hWnd);
    if(message == WM_COMMAND) return CommandFilterIDSheetDialog(hWnd, wParam);
    return 0;
}

DLLRESULT CALLBACK FilterSheetDataProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER)return UserDataSheet(hWnd, message, wParam, lParam);
    if(message == WM_INITDIALOG)return InitFilterDataSheetDialog(hWnd);
    if(message == WM_COMMAND) return CommandFilterDataSheetDialog(hWnd, wParam);
    return 0;
}


LRESULT ActivateSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //HWND activate = (HWND)wParam;
    CheckMenuItem(GetMenu(GlobalWindow), ID_SHEET, (lParam == (LPARAM)hWnd) ? MF_CHECKED : MF_UNCHECKED);
    //SendMessage(MidiClientWindow, WM_MDISETMENU, (WPARAM)hMenuInit, (LPARAM)hMenuInitWindow);
    return 0;
}


//Обработека сообщений заголовка ListBpx Рисуем заголовок
LRESULT CALLBACK WndProcHeadListViewSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    if(message == WM_ERASEBKGND)    return 0;
    if(message == WM_PAINT)         return OnPaintHeadListView(hWnd, message, wParam, lParam, uIdSubclass, dwRefData);
    if(message == HDM_LAYOUT)       return OnHeader_Layout(hWnd, message, wParam, lParam, uIdSubclass, dwRefData);

    return DefSubclassProc(hWnd, message, wParam, lParam);
}

void InitListSheet()
{
    #define Flag WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/ | LVS_REPORT | LVS_NOSORTHEADER

    RECT rc;
    GetClientRect(SheetWindow, &rc);
    ListSheet = CreateWindowEx(0, "SysListView32", "", Flag, 0, 20, rc.right - rc.left, rc.bottom - rc.top - 40, SheetWindow, (HMENU)hEdit_Sheet, hInstance, nullptr);

    if(!ListSheet)
        throw std::exception(std::string("Ошибка создания окна : ListSheet").c_str());


    hHeaderSheet = ListView_GetHeader(ListSheet);

    SetWindowSubclass(hHeaderSheet, WndProcHeadListViewSheet, 1, 48);

    SendMessage(ListSheet, WM_SETFONT, (WPARAM)Font[emFont::Font09], FALSE);
    ListView_SetExtendedListViewStyle(ListSheet, LVS_EX_HEADERINALLVIEWS | LVS_EX_DOUBLEBUFFER | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_REGIONAL | LVS_EX_FULLROWSELECT);

    for(auto a : Sheet_Collumn | boost::adaptors::indexed(0))
        AddColumn(ListSheet, a.index(), &a.value().second);

    FilterDataTimeSheet();

     #undef Flag
}


//Сохранить список листов в Exel файл
LRESULT SaveSheetList(HWND hWnd)
{
    std::string file;
    FilterSheet();

    if(bFilterData)
        file = "Sheet " + DataStartSheet + " " + DataStopSheet;
    else
        file =  "Sheet " + strYear + "-" + strMonth + "-" + strDay + "-" + strCassetteNo;

    char SaveFilename[MAX_STRING];
    strcpy_s(SaveFilename, 255, file.c_str());
    if(CmdFileSaveXlsx(hWnd, SaveFilename))
    {
        SaveSheetListXlcx(hWnd, SaveFilename, IDR_DAT4);
    }
    return 0;
}

LRESULT CommandSheet(HWND hWnd, WPARAM wParam)
{
    WORD command = LOWORD(wParam);
    if(command == 110) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FilterSheetDataProc);
    if(command == 111) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, FilterSheetIDProc);
    if(command == 112) return FilterSheet();
    if(command == 113) return SaveSheetList(hWnd);
    return 0;
}

LRESULT CALLBACK WinProcSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_NOTIFY) return OnNotifySheet(hWnd, message, wParam, lParam);
    if(message == WM_SIZE)SizeListSheet(ListSheet, lParam);
    if(message == WM_CLOSE)SheetWindow = NULL;
    if(message == WM_COMMAND)return CommandSheet(hWnd, wParam);
    if(message == WM_MDIACTIVATE)return ActivateSheet(hWnd, message, wParam, lParam);
    return DefMDIChildProc(hWnd, message, wParam, lParam);
}

void SheetInitApplication()
{
    WNDCLASSEX wc ={0};

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)WinProcSheet;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = strClassSheet.c_str();
    wc.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClassEx(&wc);

}


void SheetInitInstance()
{
    std::tm TM;

    time_t timer = time(NULL);
    localtime_s(&TM, &timer);

    char sFormat[1024];
    sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday);
    DataStopSheet = sFormat;

    timer = (time_t)difftime(timer, 60 * 60 * 24); // минус 1 день //4 месяца = 10540800
    localtime_s(&TM, &timer);
    sprintf_s(sFormat, 50, "%04d-%02d-%02d", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday);
    DataStartSheet = sFormat;

    sprintf_s(sFormat, 50, "%4d", TM.tm_year + 1900);
    strYear = sFormat;

    sprintf_s(sFormat, 50, "%2d", TM.tm_mon + 1);
    strMonth = sFormat;

    sprintf_s(sFormat, 50, "%2d", TM.tm_mday);
    strDay = sFormat;


    if(SheetWindow)
    {
        //WM_MDIACTIVATE;
        //WM_MDIRESTORE;
        //SendMessage(MidiClientWindow, WM_MDIACTIVATE, (WPARAM)OnLineWindow, 0);
        //if(OnLineWindow !=(HWND)SendMessage(MidiClientWindow, WM_MDIGETACTIVE, 0, 0))
        SetWindowPos(SheetWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        SetWindowPos(SheetWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        SendMessage(MidiClientWindow, WM_MDIACTIVATE, (WPARAM)SheetWindow, 0);
        
        return;
    }
    MDICREATESTRUCT mcs ={0};

    mcs.szTitle = strTitleSheet.c_str();
    mcs.szClass = strClassSheet.c_str();
    mcs.hOwner  = hInstance;
    mcs.x = mcs.cx = CW_USEDEFAULT;
    mcs.y = mcs.cy = CW_USEDEFAULT;
    mcs.style = WS_MAXIMIZE | MDIS_ALLCHILDSTYLES;

    SheetWindow = (HWND)SendMessage(MidiClientWindow, WM_MDICREATE, 0, (LPARAM)&mcs);
    if(!SheetWindow)
        throw std::exception(std::string("Ошибка создания окна : SheetWindow").c_str());

    InitListSheet();

    RECT rc;
    GetClientRect(SheetWindow, &rc);


    FilterHwndSheetData = CreateWindowExA(0, "BUTTON", "Фильтр по дате", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 0, 0, 150, 20, SheetWindow, (HMENU)110, hInstance, NULL);
    if(!FilterHwndSheetData)
        throw std::exception(std::string("Ошибка создания окна : Фильтр по дате").c_str());

    FilterHwndSheetID = CreateWindowExA(0, "BUTTON", "Фильтр по кассете", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 150, 0, 150, 20, SheetWindow, (HMENU)111, hInstance, NULL);
    if(!FilterHwndSheetID)
        throw std::exception(std::string("Ошибка создания окна : Фильтр по кассете").c_str());

    UpdateHwndSheet = CreateWindowExA(0, "BUTTON", "Обновить", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 300, 0, 150, 20, SheetWindow, (HMENU)112, hInstance, NULL);
    if(!UpdateHwndSheet)
        throw std::exception(std::string("Ошибка создания окна : Обновить").c_str());

    SaveHwndSheetList = CreateWindowExA(0, "BUTTON", "Сохранить", WS_CHILD | WS_VISIBLE | WS_BORDER/* | BS_FLAT*/, 450, 0, 150, 20, SheetWindow, (HMENU)113, hInstance, NULL);
    if(!SaveHwndSheetList)
        throw std::exception(std::string("Ошибка создания окна : Сохранить").c_str());

    //SELECT* FROM cassette WHERE delete_at IS NULL ORDER BY create_at DESC;

    //SendMessage(FilterHwndSheetData, WM_SETFONT, (WPARAM)Font[emFont::Font08], FALSE);
    
}

