#include "pch.h"

#include "main.h"
#include "StringData.h"

#include "Calendar.h"
#include "file.h"
#include "Cassette.h"
#include "Sheet.h"

#include "SQL.h"
#include "Exel.h"
#include "Pdf.h"



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
HWND BaseHwndSheetList = NULL;
HWND ListSheetTemp = NULL;
HWND ListCasseteTemp = NULL;
HWND FilterHwndSheet = NULL;

extern HWND FilterHwndCassette;

extern std::string DataFilterCassette;
extern std::string FilterUpdateComand;
std::string strTitleSheet = "Лист";
std::string strClassSheet = "ClassSheet";

std::string DataStartSheet = "";
std::string DataStopSheet = "";
std::string DataFilterSheet = "";

std::string strYear;
std::string strMonth;
std::string strDay;
std::string strHour = "00";
std::string strCassetteNo = "1";

BOOL bFilterData = TRUE;

int EditItem = 0;
int EditSubItem = 0;

#define LL0 130
#define LL1 100
#define LL2 90
#define LL3 380
#define L0 50
#define L1 60
#define L2 70


std::map<std::string, std::string> NamePos ={};

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
    {casSheet::NN, { "№", 60 }},
    {casSheet::Cassette, { "№", 60 }},
    {casSheet::ID, { "ID", 50 }},
    {casSheet::DataTime, { "Дата время\nсоздания листа", LL0 }},
    {casSheet::Start_at, { "Дата время\nзагрузки в печь", LL0 }},
    {casSheet::SecondPos_at, { "Дата время\nво второй зоне", LL0 }},

    {casSheet::DataTime_End, { "Дата время\nвыгрузки из печи", LL0 }},
    {casSheet::Pos, { "Текущая\nпозиция", 100 }},
    {casSheet::News, { "Кан\nтовка", L0 }},

    {casSheet::DataTime_All, { "Время закалки\nмин", L1 }},
    {casSheet::TimeForPlateHeat, { "Задание Время\nокончания нагрева", L2 }},

    {casSheet::Alloy, { "Марка стали", L1 }},
    {casSheet::Thikness, { "Толщина\nлиста\nмм", L2 }},
    {casSheet::Melt, { "Плавка", L1 }},
    {casSheet::PartNo, { "Партия", L0 }},
    {casSheet::Slab, { "Сляб", L0 }},
    {casSheet::Pack, { "Пачка", L0 }},
    {casSheet::Sheet, { "Номер\nлиста", L0 }},
    {casSheet::SubSheet, { "Номер\nпод\nлиста", L0 }},

    {casSheet::Temper, { "Заданная\nтемп-ра\nС°", LL2 }},
    {casSheet::Temperature, { "Факт\nтемп-ра\nС°", LL2 }},
    {casSheet::Speed, { "Скорость\nвыдачи\nмм/с", 80 }},

    {casSheet::Hour, { "ID листа\nЧас", L1 }},
    {casSheet::Day, { "ID листа\nДень", L1 }},
    {casSheet::Month, { "ID листа\nМесяц", L1 }},
    {casSheet::Year, { "ID листа\nГод", L1 }},
    {casSheet::CassetteNo, { "ID листа\nКасета", L1 }},
    {casSheet::SheetInCassette, { "Id листа\nНомер", L1 }},

#ifndef NODETAL
    {casSheet::PresToStartComp, { "Задание\nДавления воды", L2 }},
    {casSheet::Za_PT3, { "Давление\nводы в баке.\nбар", LL2 }},
    {casSheet::Za_TE3, { "Температура\nводы в баке.\nС°", LL1 }},
    {casSheet::LamPressTop, { "Давление\nв верхнем\nколлекторе", LL2 }},
    {casSheet::LamPressBot, { "Давление\nв нижнем\nколлекторе", LL2 }},
    {casSheet::PosClapanTop, { "Скоростная\nклапан верх", LL2 }},
    {casSheet::PosClapanBot, { "Скоростная\nклапан низ", LL2 }},

    {casSheet::Lam_TE1, { "Температура\nводы\nв поддоне. С°", LL1 }},

    {casSheet::Lam1PosClapanTop, { "Ламинарная 1\nКлапан верх", LL1 }},
    {casSheet::Lam1PosClapanBot, { "Ламинарная 1\nКлапан низ", LL1 }},
    {casSheet::Lam2PosClapanTop, { "Ламинарная 2\nКлапан верх", LL1 }},
    {casSheet::Lam2PosClapanBot, { "Ламинарная 2\nКлапан низ", LL1 }},
#endif

#ifdef _DEBUG
    {casSheet::InCant_at, { "На кантовку", LL0 }},
    {casSheet::Cant_at, { "Кантовка", LL0 }},
    {casSheet::Correct,  { "Correct", LL0 }},
    {casSheet::Diff,  { "Diff", L1 }},
    {casSheet::Pdf,  { "Pdf", LL3 }},
    {casSheet::Delete_at,  { "Удален", LL0 }},
#else
    {casSheet::Mask, { "Маскирование", 140 }},

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
#endif
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

#ifdef _DEBUG
    SetWindowPos(BaseHwndSheetList, HWND_TOP, cx - 150, 0, 0, 0, SWP_NOSIZE);
#endif
    
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

std::string FilterComand = "SELECT * FROM sheet ORDER BY start_at DESC;";



//Номер колонки в таблице листов
#pragma region //Номер колонки в таблице листов
int Col_Sheet_id = 0;
int Col_Sheet_create_at = 0;
int Col_Sheet_start_at = 0;
int Col_Sheet_datatime_end = 0;
int Col_Sheet_pos = 0;
int Col_Sheet_datatime_all = 0;
int Col_Sheet_alloy = 0;
int Col_Sheet_thikness = 0;
int Col_Sheet_melt = 0;
int Col_Sheet_slab = 0;
int Col_Sheet_partno = 0;
int Col_Sheet_pack = 0;
int Col_Sheet_sheet = 0;
int Col_Sheet_subsheet = 0;
int Col_Sheet_temper = 0;
int Col_Sheet_speed = 0;
int Col_Sheet_za_pt3 = 0;
int Col_Sheet_za_te3 = 0;
int Col_Sheet_lampresstop = 0;
int Col_Sheet_lampressbot = 0;
int Col_Sheet_posclapantop  = 0;
int Col_Sheet_posclapanbot = 0;
int Col_Sheet_mask = 0;
int Col_Sheet_lam1posclapantop = 0;
int Col_Sheet_lam1posclapanbot = 0;
int Col_Sheet_lam2posclapantop = 0;
int Col_Sheet_lam2posclapanbot = 0;
int Col_Sheet_lam_te1 = 0;
int Col_Sheet_news = 0;
int Col_Sheet_top1 = 0;
int Col_Sheet_top2 = 0;
int Col_Sheet_top3 = 0;
int Col_Sheet_top4 = 0;
int Col_Sheet_top5 = 0;
int Col_Sheet_top6 = 0;
int Col_Sheet_top7 = 0;
int Col_Sheet_top8 = 0;
int Col_Sheet_bot1 = 0;
int Col_Sheet_bot2 = 0;
int Col_Sheet_bot3 = 0;
int Col_Sheet_bot4 = 0;
int Col_Sheet_bot5 = 0;
int Col_Sheet_bot6 = 0;
int Col_Sheet_bot7 = 0;
int Col_Sheet_bot8 = 0;
int Col_Sheet_day = 0;
int Col_Sheet_month = 0;
int Col_Sheet_year = 0;
int Col_Sheet_cassetteno = 0;
int Col_Sheet_sheetincassette = 0;
int Col_Sheet_timeforplateheat = 0;
int Col_Sheet_prestostartcomp = 0;
int Col_Sheet_temperature = 0;
int Col_Sheet_correct = 0;
int Col_Sheet_pdf = 0;
int Col_Sheet_SecondPos_at = 0;
int Col_Sheet_hour = 0;
int Col_Sheet_InCant_at = 0;
int Col_Sheet_Cant_at = 0;
int Col_Sheet_Delete_at = 0;
int Col_Sheet_Cassette = 0;
#pragma endregion

//Получаем список колонов в таблице sheet
void GetColSheet(PGresult* res)
{
    if(!Col_Sheet_Delete_at)
    {
        int nFields = PQnfields(res);
        for(int j = 0; j < nFields; j++)
        {
            std::string l =  utf8_to_cp1251(PQfname(res, j));
            if(l == "id") Col_Sheet_id = j;
            else if(l == "create_at") Col_Sheet_create_at = j;
            else if(l == "start_at") Col_Sheet_start_at = j;
            else if(l == "datatime_end") Col_Sheet_datatime_end = j;
            else if(l == "pos") Col_Sheet_pos = j;
            else if(l == "datatime_all") Col_Sheet_datatime_all = j;
            else if(l == "alloy") Col_Sheet_alloy = j;
            else if(l == "thikness") Col_Sheet_thikness = j;
            else if(l == "melt") Col_Sheet_melt = j;
            else if(l == "slab") Col_Sheet_slab = j;
            else if(l == "partno") Col_Sheet_partno = j;
            else if(l == "pack") Col_Sheet_pack = j;
            else if(l == "sheet") Col_Sheet_sheet = j;
            else if(l == "subsheet") Col_Sheet_subsheet = j;
            else if(l == "temper") Col_Sheet_temper = j;
            else if(l == "speed") Col_Sheet_speed = j;
            else if(l == "za_pt3") Col_Sheet_za_pt3 = j;
            else if(l == "za_te3") Col_Sheet_za_te3 = j;
            else if(l == "lampresstop") Col_Sheet_lampresstop = j;
            else if(l == "lampressbot") Col_Sheet_lampressbot = j;
            else if(l == "posclapantop") Col_Sheet_posclapantop = j;
            else if(l == "posclapanbot") Col_Sheet_posclapanbot = j;
            else if(l == "mask") Col_Sheet_mask = j;
            else if(l == "lam1posclapantop") Col_Sheet_lam1posclapantop = j;
            else if(l == "lam1posclapanbot") Col_Sheet_lam1posclapanbot = j;
            else if(l == "lam2posclapantop") Col_Sheet_lam2posclapantop = j;
            else if(l == "lam2posclapanbot") Col_Sheet_lam2posclapanbot = j;
            else if(l == "lam_te1") Col_Sheet_lam_te1 = j;
            else if(l == "news") Col_Sheet_news = j;
            else if(l == "top1") Col_Sheet_top1 = j;
            else if(l == "top2") Col_Sheet_top2 = j;
            else if(l == "top3") Col_Sheet_top3 = j;
            else if(l == "top4") Col_Sheet_top4 = j;
            else if(l == "top5") Col_Sheet_top5 = j;
            else if(l == "top6") Col_Sheet_top6 = j;
            else if(l == "top7") Col_Sheet_top7 = j;
            else if(l == "top8") Col_Sheet_top8 = j;
            else if(l == "bot1") Col_Sheet_bot1 = j;
            else if(l == "bot2") Col_Sheet_bot2 = j;
            else if(l == "bot3") Col_Sheet_bot3 = j;
            else if(l == "bot4") Col_Sheet_bot4 = j;
            else if(l == "bot5") Col_Sheet_bot5 = j;
            else if(l == "bot6") Col_Sheet_bot6 = j;
            else if(l == "bot7") Col_Sheet_bot7 = j;
            else if(l == "bot8") Col_Sheet_bot8 = j;
            else if(l == "year") Col_Sheet_year = j;
            else if(l == "month") Col_Sheet_month = j;
            else if(l == "day") Col_Sheet_day = j;
            else if(l == "hour") Col_Sheet_hour = j;
            else if(l == "cassetteno") Col_Sheet_cassetteno = j;
            else if(l == "sheetincassette") Col_Sheet_sheetincassette = j;
            else if(l == "timeforplateheat") Col_Sheet_timeforplateheat = j;
            else if(l == "prestostartcomp") Col_Sheet_prestostartcomp = j;
            else if(l == "temperature") Col_Sheet_temperature = j;
            else if(l == "correct") Col_Sheet_correct = j;
            else if(l == "pdf") Col_Sheet_pdf = j;
            else if(l == "secondpos_at") Col_Sheet_SecondPos_at = j;
            else if(l == "incant_at")Col_Sheet_InCant_at = j;
            else if(l == "cant_at")Col_Sheet_Cant_at = j;
            else if(l == "delete_at") Col_Sheet_Delete_at = j;
            else if(l == "cassette") Col_Sheet_Cassette = j;

        }
    }
}


#ifdef _DEBUG
//#define FILTER
#ifdef FILTER
bool cmpAllAllSheet(TSheet& first, TSheet& second)
{
    return first.Diff > second.Diff;
    //return first.Start_at < second.Start_at;
}
#endif // FILTER
#endif // _DEBUG

void GetSheet(PGresult* res, TSheet& sheet, int l)
{
    sheet.id = conn_kpvl.PGgetvalue(res, l, Col_Sheet_id);

    sheet.DataTime = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_create_at));
    sheet.DataTime_End = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_datatime_end));
    sheet.Start_at = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_start_at));

    sheet.InCant_at = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_InCant_at));
    sheet.Cant_at = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_Cant_at));
    sheet.SecondPos_at = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_SecondPos_at));
    sheet.Delete_at = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_Delete_at));

    sheet.Pos = conn_kpvl.PGgetvalue(res, l, Col_Sheet_pos);

    sheet.DataTime_All = conn_kpvl.PGgetvalue(res, l, Col_Sheet_datatime_all);
    sheet.Alloy = conn_kpvl.PGgetvalue(res, l, Col_Sheet_alloy);
    sheet.Thikness = conn_kpvl.PGgetvalue(res, l, Col_Sheet_thikness);
    sheet.Melt = conn_kpvl.PGgetvalue(res, l, Col_Sheet_melt);
    sheet.Slab = conn_kpvl.PGgetvalue(res, l, Col_Sheet_slab);
    sheet.PartNo = conn_kpvl.PGgetvalue(res, l, Col_Sheet_partno);
    sheet.Pack = conn_kpvl.PGgetvalue(res, l, Col_Sheet_pack);
    sheet.Sheet = conn_kpvl.PGgetvalue(res, l, Col_Sheet_sheet);
    sheet.SubSheet = conn_kpvl.PGgetvalue(res, l, Col_Sheet_subsheet);
    sheet.Temper = conn_kpvl.PGgetvalue(res, l, Col_Sheet_temper);
    sheet.Speed = conn_kpvl.PGgetvalue(res, l, Col_Sheet_speed);

    sheet.Za_PT3 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_za_pt3);
    sheet.Za_TE3 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_za_te3);

    sheet.LamPressTop = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lampresstop);
    sheet.LamPressBot = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lampressbot);
    sheet.PosClapanTop = conn_kpvl.PGgetvalue(res, l, Col_Sheet_posclapantop);
    sheet.PosClapanBot = conn_kpvl.PGgetvalue(res, l, Col_Sheet_posclapanbot);
    sheet.Mask = conn_kpvl.PGgetvalue(res, l, Col_Sheet_mask);

    sheet.Lam1PosClapanTop = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lam1posclapantop);
    sheet.Lam1PosClapanBot = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lam1posclapanbot);
    sheet.Lam2PosClapanTop = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lam2posclapantop);
    sheet.Lam2PosClapanBot = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lam2posclapanbot);

    sheet.Lam_TE1 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_lam_te1);
    sheet.News = conn_kpvl.PGgetvalue(res, l, Col_Sheet_news);
    sheet.Top1 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top1);
    sheet.Top2 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top2);
    sheet.Top3 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top3);
    sheet.Top4 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top4);
    sheet.Top5 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top5);
    sheet.Top6 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top6);
    sheet.Top7 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top7);
    sheet.Top8 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_top8);

    sheet.Bot1 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot1);
    sheet.Bot2 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot2);
    sheet.Bot3 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot3);
    sheet.Bot4 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot4);
    sheet.Bot5 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot5);
    sheet.Bot6 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot6);
    sheet.Bot7 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot7);
    sheet.Bot8 = conn_kpvl.PGgetvalue(res, l, Col_Sheet_bot8);

    sheet.Year = conn_kpvl.PGgetvalue(res, l, Col_Sheet_year);
    sheet.Month = conn_kpvl.PGgetvalue(res, l, Col_Sheet_month);
    sheet.Day = conn_kpvl.PGgetvalue(res, l, Col_Sheet_day);
    sheet.Hour = conn_kpvl.PGgetvalue(res, l, Col_Sheet_hour);
    sheet.CassetteNo = conn_kpvl.PGgetvalue(res, l, Col_Sheet_cassetteno);
    sheet.SheetInCassette = conn_kpvl.PGgetvalue(res, l, Col_Sheet_sheetincassette);

    sheet.TimeForPlateHeat = conn_kpvl.PGgetvalue(res, l, Col_Sheet_timeforplateheat);
    sheet.PresToStartComp = conn_kpvl.PGgetvalue(res, l, Col_Sheet_prestostartcomp);
    sheet.Temperature = conn_kpvl.PGgetvalue(res, l, Col_Sheet_temperature);

    sheet.Correct = GetStringData(conn_kpvl.PGgetvalue(res, l, Col_Sheet_correct));
    sheet.Pdf = conn_kpvl.PGgetvalue(res, l, Col_Sheet_pdf);
    sheet.Cassette = conn_kpvl.PGgetvalue(res, l, Col_Sheet_Cassette);
}

std::string Old_Start_at = "";
void GetDiftDatd(TSheet& sheet)
{
    if(!sheet.Diff.length() && Old_Start_at.length())
    {
        if(Old_Start_at > sheet.Start_at)
            sheet.diff = DataTimeDiff(Old_Start_at, sheet.Start_at);
        else
            sheet.diff = DataTimeDiff(sheet.Start_at, Old_Start_at);

        std::tm TM;

        gmtime_s(&TM, &sheet.diff);
        //sheet.Diff = GetDataTimeString(TM);

        std::stringstream ss;
        //ss << std::setw(2) << std::setfill('0') << (TM.tm_year - 70) << "-";
        //ss << std::setw(2) << std::setfill('0') << (TM.tm_mon - 0) << "-";
        //ss << std::setw(2) << std::setfill('0') << (TM.tm_mday - 1) << " ";

        ss << std::setw(2) << std::setfill('0') << (TM.tm_hour + (TM.tm_mday - 1) * 24) << ":";
        ss << std::setw(2) << std::setfill('0') << TM.tm_min << ":";
        ss << std::setw(2) << std::setfill('0') << TM.tm_sec;
        std::string sd = ss.str();
        sheet.Diff = sd;
    }
    Old_Start_at = sheet.Start_at;
}

void GetCassetteId(TSheet& a)
{
    std::stringstream ssd;
    ssd << "SELECT id FROM cassette WHERE";
    ssd << " year = " << a.Year << " AND";
    ssd << " month = " << a.Month << " AND";
    ssd << " day = " << a.Day << " AND";
    if(Stoi(a.Year) >= 2024 && Stoi(a.Month) >= 8)
        ssd << " hour = " << a.Hour << " AND";
    ssd << " cassetteno = " << a.CassetteNo;
    ssd << " ORDER BY run_at DESC LIMIT 1;";
    std::string comand = ssd.str();
    PGresult* res = conn_kpvl.PGexec(comand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        if(PQntuples(res))
            a.Cassette = conn_kpvl.PGgetvalue(res, 0, 0);
    }
    else
        LOG_ERR_SQL(AllLogger, res, "");
    PQclear(res);
}

void GetCasseteId()
{
    Old_Start_at = "";
    for(auto& a : AllSheet)
    {
        GetDiftDatd(a);
        if(!Stoi(a.Cassette) && Stoi(a.Pos) > 6)
        {
            GetCassetteId(a);
            if(Stoi(a.Cassette))
            {
                std::stringstream ssd;
                ssd << "UPDATE sheet SET cassette = " << Stoi(a.Cassette) << " WHERE id = " << a.id;
                SETUPDATESQL(conn_kpvl, ssd);
            }
        }
    }
    int t = 0;
}

INT_PTR FilterSheet()
{
    int cursel = ListView_GetNextItem(ListSheet, -1, LVNI_SELECTED);


    ListView_DeleteAllItems(ListSheet);
    AllSheet.erase(AllSheet.begin(), AllSheet.end());
    PGresult* res = conn_kpvl.PGexec(FilterComand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        GetColSheet(res);
        int line = PQntuples(res);

        for(int l = 0; l < line; l++)
        {
            TSheet sheet;
            GetSheet(res, sheet, l);

            AllSheet.push_back(sheet);
            AddHistoriSheet(false);
        }
    }
    else
    {
        std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        SendDebug("conn_kpvl", errc);
        SendDebug("conn_kpvl", FilterComand);
    }
    PQclear(res);

    GetCasseteId();

    if(cursel >= 0)
    {
        int count = ListView_GetItemCount(ListSheet);
        if(count > cursel)
        {
            ListView_EnsureVisible(ListSheet, cursel, FALSE);
            ListView_SetItemState(ListSheet, cursel, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
        }
        else
        {
            ListView_EnsureVisible(ListSheet, count - 1, FALSE);
            ListView_SetItemState(ListSheet, count - 1, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
        }
    }
    EnableWindow(BaseHwndSheetList, false);
#ifdef FILTER
    std::sort(AllSheet.begin(), AllSheet.end(), cmpAllAllSheet);
#endif

return 0;
}


void FilterIDCasseteSheet()
{
    //strYear = std::to_string(stryear);
    //strMonth = std::to_string(strmonth);
    //strDay = std::to_string(strday);
    //strHour = std::to_string(strhour);
    //strCassetteNo = std::to_string(strcassetteno);

    

    std::stringstream f;
    f << "SELECT * FROM sheet WHERE ";
    if(strYear.length())
        f << "year = '" << Stoi(strYear) << "' AND ";
    if(strMonth.length())
        f << "month = '" << Stoi(strMonth) << "' AND ";
    if(strDay.length())
        f << "day = '" << Stoi(strDay) << "' AND ";
    if(strHour.length() && Stoi(strYear) >= 2024 && Stoi(strMonth) >= 8)
        f << "hour = " << Stoi(strHour) << " AND ";
    if(strCassetteNo.length())
        f << "cassetteno = " << Stoi(strCassetteNo) << " AND ";
    
#ifdef _DEBUG
    f << "true ";
#else
    f << "delete_at IS NULL ";
#endif
    f << "ORDER BY start_at DESC;";
    FilterComand = f.str();

    std::stringstream d;

    if(strYear.length())
        d << "Год = " << Stoi(strYear) << " ";
    if(strMonth.length())
        d << "Месяц = " << Stoi(strMonth) << " ";
    if(strDay.length())
        d << "День = " << Stoi(strDay) << " ";
    if(strHour.length())
        d << "Час = " << Stoi(strHour) << " ";
    if(strCassetteNo.length())
        d << "Кассета = " << Stoi(strCassetteNo);

    DataFilterSheet = d.str();
    //DataFilterSheet = FilterComand;

    SetWindowText(FilterHwndSheet, DataFilterSheet.c_str());
    //FilterComand += "year = '" + strYear + "' AND ";
    //FilterComand += "month = '" + strMonth + "' AND ";
    //FilterComand += "day = '" + strDay + "' AND ";
    //FilterComand += "cassetteno = '" + strCassetteNo + "' ";
    FilterSheet();
    
    bFilterData = FALSE;
}

void FilterDataTimeSheet()
{
    std::stringstream comand ("UPDATE sheet SET delete_at = now() WHERE delete_at IS NULL AND start_at IS NULL AND datatime_end IS NULL  AND secondpos_at IS NULL AND incant_at IS NULL AND cant_at IS NULL");
    //std::stringstream comand ("DELETE sheet WHERE start_at IS NULL AND datatime_end IS NULL AND secondpos_at IS NULL AND incant_at IS NULL AND cant_at IS NULL");
    SETUPDATESQL(conn_kpvl, comand);

    ListView_DeleteAllItems(ListSheet);
    AllSheet.erase(AllSheet.begin(), AllSheet.end());
    DataFilterSheet = "от " + DataStartSheet + " до " + DataStopSheet;
    SetWindowText(FilterHwndSheet, DataFilterSheet.c_str());

    FilterComand = "SELECT * FROM sheet ";
    FilterComand += "WHERE ";
    FilterComand += "start_at >= ";
    FilterComand += "'" + DataStartSheet + "' ";
    FilterComand += "AND start_at <= ";
    FilterComand += "'" + DataStopSheet + " 23:59:59.999' ";
    //FilterComand += "AND start_at = '2024-09-29 00:23:14'";
#ifdef _DEBUG
    FilterComand += "ORDER BY start_at DESC";
#else
    FilterComand += "AND delete_at IS NULL ";
    //FilterComand += "AND pdf IS NOT NULL AND pdf <> '' ";
    FilterComand += "ORDER BY start_at DESC";
#endif
    FilterSheet();
    bFilterData = TRUE;
}


INT_PTR CommandPassportSheetDialog(HWND hWnd, WPARAM wParam)
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

void GetCassette(PGresult* res, TCassette& cassette, int line);
void GetCassete(TSheet& p, TCassette& cassette)
{
    strYear = std::to_string(atoi(p.Year.c_str()));
    strMonth = std::to_string(atoi(p.Month.c_str()));
    strDay = std::to_string(atoi(p.Day.c_str()));
    strCassetteNo = std::to_string(atoi(p.CassetteNo.c_str()));

    std::string FilterComand1 = "SELECT * FROM cassette ";
    FilterComand1 += "WHERE ";
    FilterComand1 += "delete_at IS NULL AND ";
    FilterComand1 += "year = '" + strYear + "' AND ";
    FilterComand1 += "month = '" + strMonth + "' AND ";
    FilterComand1 += "day = '" + strDay + "' AND ";
    FilterComand1 += "cassetteno = '" + strCassetteNo + "' ";
    FilterComand1 += "ORDER BY run_at DESC";
    FilterComand1 += ";";

    PGresult* res = conn_kpvl.PGexec(FilterComand1);
    SendDebug("conn_kpvl", FilterComand1);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        int nFields = PQnfields(res);
        for(int l = 0; l < line; l++)
        {
            GetCassette(res, cassette, l);
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
    float temp = 0;
    std::string command = "SELECT create_at, content FROM todos WHERE id_name = (SELECT id_name FROM tag WHERE name = '" + name + "') AND ";
    command += "create_at >= '" + p.Start_at + "' AND ";
    command += "create_at <= '" + p.DataTime_End + "' ";
    command += "ORDER BY create_at;";
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

//BOOL CenterWindow2(HWND hwndChild, HWND hwndParent);
INT_PTR InitPassportSheetDialog(HWND hDlg, LPARAM lParam)
{
    //#define Stat03Flag WS_CHILD | WS_VISIBLE | WS_BORDER | SS_OWNERDRAW | SS_CENTERIMAGE | DT_SINGLELINE

    #define Flag WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL

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

    //GetTemperSheet(hDlg, lParam);

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

    //GetTemperCassette(hDlg, lParam);

    HWND child = GetWindow(hDlg, GW_CHILD);
    do
    {
        SendMessage(child, WM_SETFONT, (WPARAM)Font[emFont::Font11], TRUE);
    } while(child = GetWindow(child, GW_HWNDNEXT));

    return 0;
}


INT_PTR CALLBACK SheetPasportProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_INITDIALOG) return InitPassportSheetDialog(hDlg, lParam);
    if(message == WM_COMMAND) return CommandPassportSheetDialog(hDlg, wParam);
    //if(message == WM_DRAWITEM) return DrawItem((LPDRAWITEMSTRUCT)lParam);
    return 0;
}

//Клик правой кнопкой мыши
LRESULT RightClickSheet(LPNM_LISTVIEW pnm)
{
    if(pnm->iSubItem > 0)
        //pnm->iSubItem == casSheet::Pos ||
        //pnm->iSubItem == casSheet::News ||
        //pnm->iSubItem == casSheet::Year ||
        //pnm->iSubItem == casSheet::Month ||
        //pnm->iSubItem == casSheet::Day ||
        //pnm->iSubItem == casSheet::Hour ||
        //pnm->iSubItem == casSheet::CassetteNo ||
        //pnm->iSubItem == casSheet::SheetInCassette
        //)
    {
        char szBuff[1024];
        HWND hwndLV = pnm->hdr.hwndFrom;
        LV_ITEM lvi;
        lvi.iItem = pnm->iItem; // ListView_GetNextItem(hwndLV, -1, LVNI_ALL | LVNI_FOCUSED);
        if(lvi.iItem == -1) return FALSE;
        lvi.iSubItem = pnm->iSubItem;
        lvi.pszText = szBuff;
        lvi.cchTextMax = 255;
        lvi.mask = LVIF_TEXT;
        ListView_GetItem(hwndLV, &lvi);


        EditItem = pnm->iItem;
        EditSubItem = pnm->iSubItem;

        RECT r1;
        ListView_GetSubItemRect(hwndLV, EditItem, EditSubItem, LVIR_LABEL, &r1);


        if(pnm->iSubItem == casSheet::Pos)
        {
            HWND hcombo = CreateWindowEx(0, "COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top, hwndLV, (HMENU)100, hInstance, 0);
            int i=0;


            std::vector <std::string>namepos={
                "1-я часть печи",
                "2-я часть печи",
                "Закалка",
                "Охлаждение",
                "Выдача",
                "Кантовка",
                "В касете",
                "Потерян",
            };

            for(auto a : namepos)
                SendMessage(hcombo, CB_INSERTSTRING, -1, (LPARAM)a.c_str());

            int Pos = Stoi(AllSheet[EditItem].Pos);
            if(Pos <= 7)
                SendMessage(hcombo, CB_SETCURSEL, Pos - 1, 0);
            else
                SendMessage(hcombo, CB_SETCURSEL, 6, 0);

            SetWindowPos(hcombo, HWND_TOP, 0, 0, r1.right - r1.left, (r1.bottom - r1.top) * (i + 1) + 5, SWP_NOMOVE | SWP_NOZORDER);
            SendMessage(hcombo, WM_SETFONT, (WPARAM)Font[emFont::Font09], 0L);
            //OldSubProcCombo = SetWindowLongPtr(hcombo, GWLP_WNDPROC, (LONG_PTR)SubProcCombo);
            SendMessage(hcombo, CB_SHOWDROPDOWN, TRUE, 0);
            SetFocus(hcombo);
        }
        else
        {
            HWND hwEdit = CreateWindowEx(0, "EDIT", szBuff, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, r1.left, r1.top, r1.right - r1.left, r1.bottom - r1.top, hwndLV, (HMENU)100, hInstance, 0);
            //SetWindowLong(hwEdit, GWLP_USERDATA, (ULONG)this);
            SendMessage(hwEdit, WM_SETFONT, (WPARAM)Font[emFont::Font09], 0L);

            OldSubProc = SetWindowLongPtr(hwEdit, GWLP_WNDPROC, (LONG_PTR)SubProc);
            SetFocus(hwEdit);
        }
    }

    //HWND hwndLV = pnm->hdr.hwndFrom;
    //LV_ITEM lvi;
    //lvi.iItem = ListView_GetNextItem(hwndLV, -1, LVNI_ALL | LVNI_FOCUSED);
    //if(lvi.iItem == -1) return FALSE;
    //TCassette& cassette = AllCassette[lvi.iItem];
    //if(cassette.Event != "7")
    //    DisplayContextMenu(hWnd, IDR_MENU2);
    //else
    //    DisplayContextMenu(hWnd, IDR_MENU3);
    return 0;
}

void FilterUpdate();
//Двойной клик мыши
LRESULT DoubleClickSheet(HWND hWnd, LPNM_LISTVIEW pnm)
{
    HWND hwndLV = pnm->hdr.hwndFrom;
    int item = pnm->iItem;
    int subitem = pnm->iSubItem;
        //ListView_GetNextItem(hwndLV, -1, LVNI_ALL | LVNI_FOCUSED);
    if(item >= 0 && item < (int)AllSheet.size())
    {
        TSheet p = AllSheet[item];
        if(subitem == casSheet::NN || subitem == casSheet::Cassette)
        {
            FilterUpdateComand = "SELECT * FROM cassette WHERE id = " + p.Cassette;
            DataFilterCassette = "Кассета № " + p.Cassette;
            SetWindowText(FilterHwndCassette, DataFilterCassette.c_str());
            FilterUpdate();
        }
        else  if(subitem > casSheet::Cassette)
        {
            if(p.Pdf.length())
            {
                std::string url = "\\\\" + m_dbhost + "\\" + p.Pdf;
                boost::replace_all(url, "/", "\\");
                struct stat buff;
                if(!stat(url.c_str(), &buff))
                    ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
                else
                    MessageBox(hWnd, "Паспорт листа еще не сформирован", "Ошибка", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
            }
            else
                MessageBox(hWnd, "Лист еще небыл на отпуске", "Ошибка", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
        }
    }
    return 0;
}

//Вывод информации листа
LRESULT DispInfoSheet(LPARAM lParam)
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
                ELSEIF (casSheet::Cassette, p.Cassette.c_str());
                ELSEIF (casSheet::ID, p.id.c_str());
                ELSEIF (casSheet::DataTime, GetDataTimeStr(p.DataTime).c_str());
                ELSEIF (casSheet::Start_at, GetDataTimeStr(p.Start_at).c_str());
                ELSEIF (casSheet::SecondPos_at, GetDataTimeStr(p.SecondPos_at).c_str());
                
                ELSEIF (casSheet::DataTime_End, GetDataTimeStr(p.DataTime_End).c_str());
                ELSEIF (casSheet::DataTime_All, p.DataTime_All.c_str());

                else if(subItem == casSheet::Pos) lstrcpyA(plvdi->item.pszText, NamePos[p.Pos].c_str());

                ELSEIF (casSheet::Alloy, p.Alloy.c_str());
                ELSEIF (casSheet::Thikness, p.Thikness.c_str());

                ELSEIF (casSheet::Melt, p.Melt.c_str());
                ELSEIF (casSheet::Slab, p.Slab.c_str());
                ELSEIF (casSheet::PartNo, p.PartNo.c_str());
                ELSEIF (casSheet::Pack, p.Pack.c_str());
                ELSEIF (casSheet::Sheet, p.Sheet.c_str());
                ELSEIF (casSheet::SubSheet, p.SubSheet.c_str());

                ELSEIF (casSheet::Temper, p.Temper.c_str());
                ELSEIF (casSheet::Temperature, p.Temperature.c_str());
                ELSEIF (casSheet::Speed, p.Speed.c_str());
#ifndef NODETAL
                ELSEIF (casSheet::Za_PT3, p.Za_PT3.c_str());
                ELSEIF (casSheet::Za_TE3, p.Za_TE3.c_str());
                ELSEIF (casSheet::LamPressTop, p.LamPressTop.c_str());
                ELSEIF (casSheet::LamPressBot, p.LamPressBot.c_str());
                ELSEIF (casSheet::PosClapanTop, p.PosClapanTop.c_str());
                ELSEIF (casSheet::PosClapanBot, p.PosClapanBot.c_str());
                ELSEIF (casSheet::Lam1PosClapanTop, p.Lam1PosClapanTop.c_str());
                ELSEIF (casSheet::Lam1PosClapanBot, p.Lam1PosClapanBot.c_str());
                ELSEIF (casSheet::Lam2PosClapanTop, p.Lam2PosClapanTop.c_str());
                ELSEIF (casSheet::Lam2PosClapanBot, p.Lam2PosClapanBot.c_str());
                ELSEIF (casSheet::Lam_TE1, p.Lam_TE1.c_str());
                ELSEIF (casSheet::News, Stoi(p.News) ? "Да" : "Нет");
                ELSEIF (casSheet::PresToStartComp, p.PresToStartComp.c_str());  //Уставка давления для запуска комперссора
#endif
                ELSEIF (casSheet::Year, p.Year.c_str());
                ELSEIF (casSheet::Month, p.Month.c_str());
                ELSEIF (casSheet::Day, p.Day.c_str());
                ELSEIF (casSheet::Hour, p.Hour.c_str());
                ELSEIF (casSheet::CassetteNo, p.CassetteNo.c_str());
                ELSEIF (casSheet::SheetInCassette, p.SheetInCassette.c_str());
                ELSEIF (casSheet::TimeForPlateHeat, p.TimeForPlateHeat.c_str()); //Время сигнализации окончания нагрева, мин


#ifdef _DEBUG
                ELSEIF (casSheet::Correct, GetDataTimeStr(p.Correct).c_str());  //
                ELSEIF (casSheet::Pdf, p.Pdf.c_str());  //
                ELSEIF (casSheet::InCant_at, GetDataTimeStr(p.InCant_at).c_str());
                ELSEIF (casSheet::Cant_at, GetDataTimeStr(p.Cant_at).c_str());
                ELSEIF (casSheet::Diff, p.Diff.c_str());

                ELSEIF (casSheet::Delete_at, GetDataTimeStr(p.Delete_at).c_str());
#else
                ELSEIF (casSheet::Mask, p.Mask.c_str());
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

#endif
                //ELSEIF (casSheet::TempWatTankCool, p.TempWatTankCool.c_str());  //Температура закалочной воды для вкл.охлаждения

            }
        }
    }
    return 0;
}



//LRESULT OldSubProcCombo = NULL;
//LRESULT APIENTRY SubProcCombo(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    //if(message == CBN_SELCHANGE)return DestroyWindow(hWnd);
//    if(message == WM_KILLFOCUS)
//    {
//        DestroyWindow(hWnd);
//        InvalidateRect(GetParent(hWnd), NULL, 0);
//        //return 0;
//    }
//    //if(message == WM_COMMAND)
//    //{
//    //        if(((WORD)(((DWORD_PTR)(wParam)) & 0xffff)) == CBN_KILLFOCUS);
//    //        {
//    //            //HWND hwndComboBox = (HWND)lParam;
//    //            DestroyWindow(hWnd);
//    //        }
//    //}
//    return CallWindowProc((WNDPROC)OldSubProcCombo, hWnd, message, wParam, lParam);
//}


LRESULT ListSheetSubCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    HWND hwndComboBox = (HWND)lParam;
    WORD command = HIWORD(wParam);
    switch(command)
    {
        //case CBN_SELCHANGE:
        //{
        //    int t = 0;
        //    break;
        //}
        //case CBN_CLOSEUP:
        //{
        //    int t = 0;
        //    break;
        //}
        case CBN_SELENDOK:
        {

            LPARAM t = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);
            SendMessage(hWnd, WM_USER, USER_COMBO_COMMAND, t);
            //int tt = 0;
        }
        case CBN_SELENDCANCEL:
        {
            //int t = 0;
        }
        case CBN_KILLFOCUS:
        {
            DestroyWindow(hwndComboBox);
        }
        break;
    }
    return 0;
}

LRESULT LeftClickSheet(LPNM_LISTVIEW pnm)
{

    return 0;
}


LRESULT OnNotifySheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNM_LISTVIEW  pnm = (LPNM_LISTVIEW)lParam;

#ifdef _DEBUG
    if(pnm->hdr.code == NM_CLICK)return LeftClickSheet(pnm); else 
    if(pnm->hdr.code == NM_RCLICK)return RightClickSheet(pnm); else 
#endif
    if(pnm->hdr.code == NM_DBLCLK) return DoubleClickSheet(hWnd, pnm); else 
    if(pnm->hdr.code == LVN_GETDISPINFO)DispInfoSheet(lParam);
    return 0;
}

INT_PTR InitFilterDataSheetDialog(HWND hWnd)
{
    CenterWindow(hWnd, SheetWindow);
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), DataStartSheet.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), DataStopSheet.c_str());
    return 0;
}

INT_PTR InitFilterIDSheetDialog(HWND hWnd)
{
    CenterWindow(hWnd, SheetWindow);
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), strYear.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), strMonth.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT3), strDay.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT4), strHour.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT5), strCassetteNo.c_str());
    return 0;
}

INT_PTR CommandFilterDataSheetDialog(HWND hWnd, WPARAM wParam)
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

INT_PTR CommandFilterIDSheetDialog(HWND hWnd, WPARAM wParam)
{
    if(wParam == IDC_BUTTON1)
    {
        CreateCalendar(hWnd, 1);
    }
    else if(wParam == IDOK)
    {
        //std::string datatest;
        char ssYear[255];
        char ssMonth[255];
        char ssDay[255];
        char ssHour[255];
        char ssCassetteNo[255];
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT3), ssDay, 255);
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), ssMonth, 255);
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT1), ssYear, 255);
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT4), ssHour, 255);    

        GetWindowText(GetDlgItem(hWnd, IDC_EDIT5), ssCassetteNo, 255);
        //int hour = Stoi(ssHour);
        //if(TestData(datatest))
        {
            strYear = ssYear;
            strMonth = ssMonth;
            strDay = ssDay;
            strHour = ssHour;
            strCassetteNo = ssCassetteNo;
        }
        //else
        //    return MessageBox(hWnd, (std::string(errordata) + " " + datatest).c_str(), "Ошибка", 0);


        FilterIDCasseteSheet();

        EndDialog(hWnd, FALSE);
    }
    else if(wParam == IDCANCEL)
    {
        EndDialog(hWnd, FALSE);
    }
    return 0;
}


INT_PTR UserDataSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(wParam == 1)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];
        sprintf_s(sFormat, 50, "%02d-%02d-%04d", TM->wDay, TM->wMonth, TM->wYear);
        DataStartSheet = sFormat;
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), DataStartSheet.c_str());
    }
    if(wParam == 2)
    {
        SYSTEMTIME* TM = (SYSTEMTIME*)lParam;
        if(!TM) return 0;

        char sFormat[50];
        sprintf_s(sFormat, 50, "%02d-%02d-%04d", TM->wDay, TM->wMonth, TM->wYear);
        DataStopSheet = sFormat;

        SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), DataStopSheet.c_str());
    }
    return 0;
}


INT_PTR UserIDSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        SetWindowText(GetDlgItem(hWnd, IDC_EDIT3), strDay.c_str());
    }

    return 0;
}

INT_PTR CALLBACK FilterSheetIDProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER)return UserIDSheet(hWnd, message, wParam, lParam);
    if(message == WM_INITDIALOG)return InitFilterIDSheetDialog(hWnd);
    if(message == WM_COMMAND) return CommandFilterIDSheetDialog(hWnd, wParam);
    return 0;
}

INT_PTR CALLBACK FilterSheetDataProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT OldListSheetSubProc = NULL;


std::map <int, std::string>MapCollSheet ={
    {casSheet::Cassette, "cassette"},
    {casSheet::DataTime, "datatime"},
    {casSheet::Start_at, "start_at"},
    {casSheet::SecondPos_at, "secondpos_at"},
    {casSheet::DataTime_End, "datatime_end"},

    {casSheet::Pos, "pos"},
    {casSheet::News, "news"},

    {casSheet::TimeForPlateHeat, "timeforplateheat"},
    {casSheet::DataTime_All, "datatime_all"},

    //{casSheet::Alloy, "alloy"},
    //{casSheet::Thikness, "thikness"},
    //{casSheet::Melt, "melt"},
    //{casSheet::Slab, "slab"},
    //{casSheet::PartNo, "partno"},
    //{casSheet::Pack, "pack"},
    //{casSheet::Sheet, "sheet"},
    //{casSheet::SubSheet, "subSheet"},

    {casSheet::Year, "year"},
    {casSheet::Month, "month"},
    {casSheet::Day, "day"},
    {casSheet::Hour, "hour"},
    {casSheet::CassetteNo, "cassetteno"},
    {casSheet::SheetInCassette, "sheetincassette"},


    {casSheet::Temper, "temper"},
    {casSheet::Temperature, "temperature"},
    {casSheet::Speed, "speed"},

#ifndef NODETAL
    {casSheet::PresToStartComp, "prestostartcomp"},
    {casSheet::Za_PT3, "za_pt3"},
    {casSheet::Za_TE3, "za_te3"},

    {casSheet::LamPressTop, "lampresstop"},
    {casSheet::LamPressBot, "lampressbot"},
    {casSheet::PosClapanTop, "posclapantop"},
    {casSheet::PosClapanBot, "posclapanbot"},

    {casSheet::Lam1PosClapanTop, "lam1posclapantop"},
    {casSheet::Lam1PosClapanBot, "lam1posclapanbot"},
    {casSheet::Lam2PosClapanTop, "lam2posclapantop"},
    {casSheet::Lam2PosClapanBot, "lam2posclapanbot"},

    {casSheet::Lam_TE1, ""},
#endif

#ifdef _DEBUG
    {casSheet::InCant_at, "incant_at"},
    {casSheet::Cant_at, "cant_at"},
    {casSheet::Correct, "correct"},
    {casSheet::Pdf, "pdf"},
    {casSheet::Delete_at, "delete_at"},
#else
        {casSheet::Mask, "mask"},
        {casSheet::Top1, "toot1"},
        {casSheet::Top2, "toot2"},
        {casSheet::Top3, "toot3"},
        {casSheet::Top4, "toot4"},
        {casSheet::Top5, "toot5"},
        {casSheet::Top6, "toot6"},
        {casSheet::Top7, "toot7"},
        {casSheet::Top8, "toot8"},

        {casSheet::Bot1, "boot1"},
        {casSheet::Bot2, "boot2"},
        {casSheet::Bot3, "boot3"},
        {casSheet::Bot4, "boot4"},
        {casSheet::Bot5, "boot5"},
        {casSheet::Bot6, "boot6"},
        {casSheet::Bot7, "boot7"},
        {casSheet::Bot8, "boot8"},
#endif
};


void SaveUpdateSheetLog(std::stringstream& ssd, std::string old)
{
    std::fstream fC;
    fC = std::fstream("UpdateSheetLog.txt", std::fstream::binary | std::fstream::out | std::ios::app);
    fC << GetDataTimeString() << " [" << MyName << "] (Old = " << old << ") " << ssd.str() << ";" << std::endl;
    fC.close();

}


bool UpdateSheet1(HWND hWnd, std::string& vv, std::string ss, TSheet& p, int upd)
{
    std::stringstream ssd;

    if(vv == ss) return true;
    if(MapCollSheet[upd].length())
    {
        std::string old = vv;
        vv = ss;
        
        if(MapCollSheet[upd] == "delete_at")
        {
            if(vv.length())
                ssd << "UPDATE sheet SET delete_at = '" << vv << "'  WHERE id = " << p.id;
            else
                ssd << "UPDATE sheet SET delete_at = DEFAULT WHERE id = " << p.id;
        }
        else if(MapCollSheet[upd] == "pdf")
        {
            if(vv.length())
                ssd << "UPDATE sheet SET pdf = '" << vv << "' WHERE id = " << p.id;
            else
                ssd << "UPDATE sheet SET pdf = DEFAULT WHERE id = " << p.id;
        }
        else
        {
            if(vv.length())
                ssd << "UPDATE sheet SET pdf = DEFAULT, " << MapCollSheet[upd] << " = '" << vv << "' WHERE id = " << p.id;
            else
                ssd << "UPDATE sheet SET pdf = DEFAULT, " << MapCollSheet[upd] << " = DEFAULT WHERE id = " << p.id;
        }
        SaveUpdateSheetLog(ssd, old);
        SETUPDATESQL(conn_kpvl, ssd);
    }
    else
        MessageBox(hWnd, "Поле нельзя редактировать!", "Внимание!", MB_SYSTEMMODAL | MB_ICONWARNING | MB_OK);
    return true;
}

bool UpdateSheet2(HWND hWnd, std::string& vv, std::string ss, TSheet& p, int upd)
{
    std::stringstream ssd;

    if(vv == ss) return true;
    if(MapCollSheet[upd].length())
    {
        std::string old = vv;
        vv = ss;
        if(MapCollSheet[upd] == "delete_at")
        {
            if(vv.length())
                ssd << "UPDATE sheet SET delete_at = '" << vv << "'  WHERE id = " << p.id;
            else
                ssd << "UPDATE sheet SET delete_at = DEFAULT WHERE id = " << p.id;
        }
        else if(MapCollSheet[upd] == "pdf")
        {
            if(vv.length())
                ssd << "UPDATE sheet SET pdf = '" << vv << "' WHERE id = " << p.id;
            else
                ssd << "UPDATE sheet SET pdf = DEFAULT WHERE id = " << p.id;
        }
        else
        {
            if(vv.length())
                ssd << "UPDATE sheet SET pdf = DEFAULT, " << MapCollSheet[upd] << " = " << vv << " WHERE id = " << p.id;
            else
                ssd << "UPDATE sheet SET pdf = DEFAULT, " << MapCollSheet[upd] << " = DEFAULT WHERE id = " << p.id;
        }
        SaveUpdateSheetLog(ssd, old);
        SETUPDATESQL(conn_kpvl, ssd);
    }
    else
        MessageBox(hWnd, "Поле нельзя редактировать!", "Внимание!", MB_SYSTEMMODAL | MB_ICONWARNING | MB_OK);
    return true;
}
//#define EES1(_ss, _u) EditSubItem == Cassete::_ss) UpdateSheet(GetDataTimeStr2(ss), _u
//#define EES2(_ss, _u) CassetteSubItem == Cassete::_ss) UpdateSheet(ss, _u


LRESULT ListSheetSubProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_USER && wParam == USER_COMBO_COMMAND)
    {
        if(EditItem >= 0 && EditItem < AllSheet.size())
        {
            TSheet& p = AllSheet[EditItem];
            int Pos = (int)lParam + 1;
            if(Pos <= 7)
                UpdateSheet1(hWnd, p.Pos, std::to_string(Pos), p, casSheet::Pos);
            else 
                if(Stoi(p.Pos) <= 7) UpdateSheet1(hWnd, p.Pos, "10", p, casSheet::Pos);
        }
    }
    if(message == WM_USER && wParam == USER_EDIT_COMMAND && lParam)
    {
        char* buff = (char*)lParam;

        if(EditItem >= 0 && EditItem < AllSheet.size())
        {
            TSheet& p = AllSheet[EditItem];
#define ISEDIT0(_e) EditSubItem == casSheet::_e) UpdateSheet1(hWnd, p._e, GetDataTimeStr2(buff), p, casSheet::_e
#define ISEDIT1(_e) EditSubItem == casSheet::_e) UpdateSheet1(hWnd, p._e, buff, p, casSheet::_e
#define ISEDIT2(_e) EditSubItem == casSheet::_e) UpdateSheet2(hWnd, p._e, buff, p, casSheet::_e

            if(ISEDIT2(Cassette));

            else if(ISEDIT0(DataTime));
            else if(ISEDIT0(Start_at));
            else if(ISEDIT0(SecondPos_at));
            else if(ISEDIT0(DataTime_End));
            else if(ISEDIT2(TimeForPlateHeat));
            else if(ISEDIT2(DataTime_All));

            //else if(ISEDIT(Alloy));
            //else if(ISEDIT(Thikness));
            //else if(ISEDIT(Melt));
            //else if(ISEDIT(Slab));
            //else if(ISEDIT(PartNo));
            //else if(ISEDIT(Pack));
            //else if(ISEDIT(Sheet));
            //else if(ISEDIT(SubSheet));

            else if(ISEDIT2(News));
            else if(ISEDIT1(Year));
            else if(ISEDIT1(Month));
            else if(ISEDIT1(Day));
            else if(ISEDIT2(Hour));
            else if(ISEDIT2(CassetteNo));
            else if(ISEDIT2(SheetInCassette));

            else if(ISEDIT2(Temper));
            else if(ISEDIT2(Temperature));
            else if(ISEDIT2(Speed));

#ifndef NODETAL
            else if(ISEDIT2(PresToStartComp));
            else if(ISEDIT2(Za_PT3));
            else if(ISEDIT2(Za_TE3));
            else if(ISEDIT2(LamPressTop));
            else if(ISEDIT2(LamPressBot));
            else if(ISEDIT2(PosClapanTop));
            else if(ISEDIT2(PosClapanBot));
            else if(ISEDIT2(Lam1PosClapanTop));
            else if(ISEDIT2(Lam1PosClapanBot));
            else if(ISEDIT2(Lam2PosClapanTop));
            else if(ISEDIT2(Lam2PosClapanBot));

            else if(ISEDIT2(Lam_TE1));
#endif

#ifdef _DEBUG
            else if(ISEDIT0(InCant_at));
            else if(ISEDIT0(Cant_at));
            else if(ISEDIT0(Correct));
            else if(ISEDIT1(Pdf));
            else if(ISEDIT0(Delete_at));
#else
            else if(ISEDIT1(Mask));

            else if(ISEDIT2(Top1));
            else if(ISEDIT2(Top2));
            else if(ISEDIT2(Top3));
            else if(ISEDIT2(Top4));
            else if(ISEDIT2(Top5));
            else if(ISEDIT2(Top6));
            else if(ISEDIT2(Top7));
            else if(ISEDIT2(Top8));

            else if(ISEDIT2(Bot1));
            else if(ISEDIT2(Bot2));
            else if(ISEDIT2(Bot3));
            else if(ISEDIT2(Bot4));
            else if(ISEDIT2(Bot5));
            else if(ISEDIT2(Bot6));
            else if(ISEDIT2(Bot7));
            else if(ISEDIT2(Bot8));
#endif // DEBUG

            else
                MessageBox(hWnd, "Поле нельзя редактировать!", "Внимание!", MB_SYSTEMMODAL | MB_ICONWARNING | MB_OK);
#undef ISEDIT
        }

        GlobalUnlock((HGLOBAL)lParam);
        GlobalFree((HGLOBAL)lParam);
    }
    if(message == WM_COMMAND)ListSheetSubCommand(hWnd, wParam, lParam);

    return CallWindowProc((WNDPROC)OldListSheetSubProc, hWnd, message, wParam, lParam);
}


void InitListSheet()
{
    #define FlagSheet WS_CHILD | WS_VISIBLE | WS_BORDER /*| WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/ | LVS_REPORT | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED /*| LVS_SINGLESEL*/

    RECT rc;
    GetClientRect(SheetWindow, &rc);
    ListSheet = CreateWindowEx(0, "SysListView32", "", FlagSheet, 0, 20, rc.right - rc.left, rc.bottom - rc.top - 40, SheetWindow, (HMENU)hEdit_Sheet, hInstance, nullptr);

    if(!ListSheet)
        throw std::exception(std::string("Ошибка создания окна : ListSheet").c_str());

    OldListSheetSubProc = SetWindowLongPtr(ListSheet, GWLP_WNDPROC, (LONG_PTR)ListSheetSubProc);

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
        SaveSheetListXlcx(hWnd, SaveFilename, IDR_DAT1);
    }
    return 0;
}


LRESULT BaseSheetList(HWND hWnd)
{
    if(MessageBox(hWnd, "Вы уверяны что хотите сохранить изменения в базу?", "Внимание!", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        
        FilterSheet();
    }

    return 0;
}

LRESULT CommandSheet(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    WORD command = HIWORD(wParam);
    WORD commandId = LOWORD(wParam);
    if(commandId == 110) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, FilterSheetDataProc);
    if(commandId == 111) return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, FilterSheetIDProc);
    if(commandId == 112) return FilterSheet();
    if(commandId == 113) return SaveSheetList(hWnd);
    if(commandId == 115) return BaseSheetList(hWnd);

    
    return 0;
}


LRESULT DrawItemSheet(HWND, UINT, WPARAM, LPARAM lParam)
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

    if(lvi.iItem >= 0 && lvi.iItem < (int)AllSheet.size())
    {
        TSheet& p = AllSheet[lvi.iItem];

        int pos = Stoi(p.Pos);

        BOOL bSelected=lvi.state & LVIS_SELECTED;
        bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

        LISTPAINT::GetItemRect(lpdis->hwndItem, lpdis->itemID, &rc, LISTPAINT::Bounds);
        SetBkMode(lpdis->hDC, 1);

        COLORREF clrTextSave = SetTextColor(lpdis->hDC, RGB(0, 0, 0));
        
        if(bSelected)
        {
            FillRect(lpdis->hDC, &rc, TitleBrush4);
            if(p.Edit)
                clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 255, 0));
            else if(p.Delete_at.length())
                clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 0, 0));
            else
                clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 255, 255));
        }
        else
        {
            if(p.Delete_at.length())
            {
                clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 0, 0));
            }
            else
            {
                if(p.Edit)
                {
                    clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 255, 0));
                }
                else if(p.Delete_at.length())
                {
                    clrTextSave = SetTextColor(lpdis->hDC, RGB(255, 0, 0));
                }
#ifdef _DEBUG
                if(p.diff && p.diff < 60)
                {
                    FillRect(lpdis->hDC, &rc, TitleBrush11);
                }
                else
#endif // DEBUG
                    if(pos < 7)
                    {
                        FillRect(lpdis->hDC, &rc, TitleBrush13);
                    }
                    else
                        if(pos == 7)
                        {
                            if(p.Pdf.length())
                            {
#ifdef _DEBUG
                                if(!p.Correct.length())
                                    FillRect(lpdis->hDC, &rc, TitleBrush6); //Светло зеленая заливка
                                else
#endif // _DEBUG
                                    if(lvi.iItem % 2)
                                    {
                                        FillRect(lpdis->hDC, &rc, TitleBrush5);
                                    }
                                    else
                                    {
                                        FillRect(lpdis->hDC, &rc, TitleBrush1);
                                    }

                                    //{
                                    //    FillRect(lpdis->hDC, &rc, TitleBrush9); //темно зеленая заливка
                                    //}

                            }
                            else if(p.Correct.length())
                            {
                                FillRect(lpdis->hDC, &rc, TitleBrush3); //светлосиняя заливка
                            }
                            else
                            {
                                FillRect(lpdis->hDC, &rc, TitleBrush7); //Светло желтая заливка
                            }
                        }
                        else if(pos >= 10)
                        {
                            FillRect(lpdis->hDC, &rc, TitleBrush8);
                        }
                        else if(lvi.iItem % 2)
                        {
                            FillRect(lpdis->hDC, &rc, TitleBrush5);
                        }
                        else
                        {
                            FillRect(lpdis->hDC, &rc, TitleBrush1);
                        }
            }
        }


        //ImageList_DrawEx(hIL, lvi.iImage, lpdis->hDC, rc.left, rc.top, 16, 16, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT);

        lvc.mask=LVCF_FMT | LVCF_WIDTH;

        SelectObject(lpdis->hDC, Font[emFont::Font10]);

        //if(bSelected)
        //{
            //rc.bottom -= 1;
            //rc.right -= 1;
            //DrawFrameControl(lpdis->hDC, &rc, DFC_BUTTON, DFCS_BUTTONPUSH);
            //DrawFocusRect(lpdis->hDC, &rc);
            //FrameRect(lpdis->hDC, &rc, TitleBrush0);
        //}

        for(int nColumn=0; LISTPAINT::GetColumn(lpdis->hwndItem, nColumn, &lvc); nColumn++)
        {
            ListView_GetItemText(lpdis->hwndItem, lpdis->itemID, nColumn, szBuff, sizeof(szBuff));
            //nJustify = DT_LEFT;

            ListView_GetSubItemRect(lpdis->hwndItem, lpdis->itemID, nColumn, LVIR_LABEL, &rcLabel);

            
            if(nColumn >= casSheet::Melt && nColumn <= casSheet::SubSheet)
            {
                RECT rcLabel2;
                ListView_GetSubItemRect(lpdis->hwndItem, lpdis->itemID, casSheet::SubSheet, LVIR_LABEL, &rcLabel2);
                rcLabel2.left  = rcLabel.left +1;
                FrameRect(lpdis->hDC, &rcLabel2, TitleBrush0);
            }
            else if(nColumn >= casSheet::Year && nColumn <= casSheet::SheetInCassette)
            {
                RECT rcLabel2;
                ListView_GetSubItemRect(lpdis->hwndItem, lpdis->itemID, casSheet::SheetInCassette, LVIR_LABEL, &rcLabel2);
                rcLabel2.left  = rcLabel.left + 1;
                FrameRect(lpdis->hDC, &rcLabel2, TitleBrush0);
            }


#ifndef _DEBUG
            if(nColumn >= casSheet::Top1 && nColumn <= casSheet::Bot8)
            {
                if(Stoi(szBuff))
                {
                    clrTextSave = SetTextColor(lpdis->hDC, RGB(0, 0, 0));
                    FillRect(lpdis->hDC, &rcLabel, TitleBrush6);
                }
                else
                {
                    clrTextSave = SetTextColor(lpdis->hDC, RGB(0, 0, 0));
                    FillRect(lpdis->hDC, &rcLabel, TitleBrush7);
                }
            }
#endif // DEBUG
            //rcLabel.left += 1;
            //rcLabel.right -= 1;


            rcLabel.left += 2;
            rcLabel.right -= 2;


            pszText=LISTPAINT::MakeShortString(lpdis->hDC, szBuff, rcLabel.right - rcLabel.left, 0);

            DrawText(lpdis->hDC, pszText, -1, &rcLabel, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);
        }

        if(bSelected)
        {
            rc.bottom -= 1;
            rc.right -= 1;
            //DrawFrameControl(lpdis->hDC, &rc, DFC_BUTTON, DFCS_BUTTONPUSH);
            DrawFocusRect(lpdis->hDC, &rc);
            //FrameRect(lpdis->hDC, &rc, TitleBrush0);
        }
        
        //if(bSelected)SetBkColor(lpdis->hDC, (ULONG)clrBkSave);
        //SetTextColor(lpdis->hDC, (ULONG)clrTextSave);
    }
    return TRUE;
}

LRESULT CALLBACK WinProcSheet(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_NOTIFY) return OnNotifySheet(hWnd, message, wParam, lParam);
    if(message == WM_SIZE)SizeListSheet(ListSheet, lParam);
    if(message == WM_CLOSE)SheetWindow = NULL;
    if(message == WM_COMMAND)return CommandSheet(hWnd, wParam, lParam);
    if(message == WM_MDIACTIVATE)return ActivateSheet(hWnd, message, wParam, lParam);
    if(message == WM_DRAWITEM)return DrawItemSheet(hWnd, message, wParam, lParam);
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
    //try
    {
//#ifdef _DEBUG
//    DataStartSheet = "01-08-2024";
//    DataStopSheet =  "01-10-2024";
//    strYear = "2024";
//    strMonth = "08";
//    strDay = "01";
//#else
    //std::tm TM;
    //
    //time_t timer = time(NULL);
    //localtime_s(&TM, &timer);
    //
    //char sFormat[1024];
    //sprintf_s(sFormat, 50, "%02d-%02d-%04d", TM.tm_mday, TM.tm_mon + 1, TM.tm_year + 1900);
    //DataStopSheet = sFormat;
    //
    //timer = (time_t)difftime(timer, 60 * 60 * 24 * 7); // минус 7 дней //4 месяца = 10540800
    //localtime_s(&TM, &timer);
    //sprintf_s(sFormat, 50, "%02d-%02d-%04d", TM.tm_mday, TM.tm_mon + 1, TM.tm_year + 1900);
    //DataStartSheet = sFormat;
    //
    //sprintf_s(sFormat, 50, "%4d", TM.tm_year + 1900);
    //strYear = sFormat;
    //
    //sprintf_s(sFormat, 50, "%2d", TM.tm_mon + 1);
    //strMonth = sFormat;
    //
    //sprintf_s(sFormat, 50, "%2d", TM.tm_mday);
    //strDay = sFormat;

        std::string comand = "SELECT TO_CHAR((now() - INTERVAL '7 day'), 'DD-MM-YYYY'), TO_CHAR(now(), 'DD-MM-YYYY'), now();";
        PGresult* res = conn_kpvl.PGexec(comand);
        if(PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res))
        {
            DataStartSheet = GetStringData(conn_kpvl.PGgetvalue(res, 0, 0));
            DataStopSheet = GetStringData(conn_kpvl.PGgetvalue(res, 0, 1));;
            std::string data = GetStringData(conn_kpvl.PGgetvalue(res, 0, 2));;
            std::tm TM;
            DataTimeOfString(data, TM);
            strYear = (boost::format("%|04|") % (TM.tm_year)).str();
            strMonth = (boost::format("%|02|") % (TM.tm_mon)).str();
            strDay = (boost::format("%|02|") % (TM.tm_mday)).str();
            PQclear(res);
        }
        else
        {
            PQclear(res);
            throw std::exception("Ошибка связи с базой");
        }

    //#endif


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

        FilterHwndSheet = CreateWindowExA(0, "EDIT", DataFilterSheet.c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER | ES_AUTOHSCROLL/* | BS_FLAT*/, 610, 0, 540, 20, SheetWindow, (HMENU)114, hInstance, NULL);
        if(!FilterHwndSheet)
            throw std::exception(std::string("Ошибка создания окна : Филтер").c_str());

#ifdef _DEBUG
        BaseHwndSheetList = CreateWindowExA(0, "BUTTON", "Сохранить в базу", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED/* | BS_FLAT*/, 0, 0, 150, 20, SheetWindow, (HMENU)115, hInstance, NULL);
        if(!BaseHwndSheetList)
            throw std::exception(std::string("Ошибка создания окна : BaseHwndSheetList").c_str());
#endif

    //SELECT* FROM cassette WHERE delete_at IS NULL ORDER BY create_at DESC;

    //SendMessage(FilterHwndSheetData, WM_SETFONT, (WPARAM)Font[emFont::Font08], FALSE);
    }
    //CATCH(AllLogger, "");
}

