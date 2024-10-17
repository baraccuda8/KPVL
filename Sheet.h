#pragma once

//enum emSheet{
//    create_at = 0,
//    zone = 1,
//    pos = 2,
//    id,
//    datatime_end,
//    datatime_all,
//    alloy,
//    thikness,
//    melt,
//    partno,
//    pack,
//    sheet,
//    temper,
//    speed,
//    za_pt3,
//    za_te3,
//    lampresstop,
//    lampressbot,
//    posclapantop,
//    posclapanbot,
//    mask,
//    lam1posclapantop,
//    lam1posclapanbot,
//    lam2posclapantop,
//    lam2posclapanbot,
//    lam_te1,
//    news,
//    top1,
//    top2,
//    top3,
//    top4,
//    top5,
//    top6,
//    top7,
//    top8,
//    bot1,
//    bot2,
//    bot3,
//    bot4,
//    bot5,
//    bot6,
//    bot7,
//    bot8,
//    day,
//    month,
//    year,
//    cassetteno,
//    sheetincassette,
//    start_at,
//    timeforplateheat,
//    prestostartcomp,
//    //tempwattankcool,
//};

typedef struct TSheet{

    bool Edit = false;
    time_t diff = 0;
    std::string Diff = "";
    std::string Cassette = "";          //ИД связанной касеты
        
    std::string id = "";                //Уникальный номер
    std::string DataTime = "";          //Дата, время создание листа
    std::string Start_at = "";          //Дата, время загрузки листа в закалочную печь
    std::string DataTime_End = "";      //Дата, время выдачи листа из закалочной печи
    std::string Pos = "";               //Текущая позиция листа
    std::string DataTime_All = "";      //Продолжительность закалки, мин
    //std::string Zone = "";              //
    std::string Alloy = "";             //Марка стали
    std::string Thikness = "";          //Толщина листа, мм
    std::string Melt = "";              //Плавка
    std::string Slab = "";              //Номер сляба
    std::string PartNo = "";            //Партия
    std::string Pack = "";              //Пачка
    std::string Sheet = "";             //Номер листа
    std::string SubSheet = "";          //Номер подлиста
    std::string Temper = "";            //Уставка температуры
    std::string Speed = "";             //Скорость выгрузки

    std::string Za_PT3 = "";            //Давление воды в баке
    std::string Za_TE3 = "";            //Температура воды в баке

    std::string LaminPressTop = "";     //Давление в верхнем коллекторе
    std::string LaminPressBot = "";     //Давление в нижнем коллекторе
    std::string PosClapanTop = "";      //Клапан. Скоростная секция. Верх
    std::string PosClapanBot = "";      //Клапан. Скоростная секция. Низ
    std::string Mask = "";              //Режим работы клапана

    std::string Lam1PosClapanTop = "";  //Клапан. Ламинарная секция 1. Верх
    std::string Lam1PosClapanBot = "";  //Клапан. Ламинарная секция 1. Низ
    std::string Lam2PosClapanTop = "";  //Клапан. Ламинарная секция 2. Верх
    std::string Lam2PosClapanBot = "";  //Клапан. Ламинарная секция 2. Низ

    std::string LAM_TE1 = "";           //Температура воды в поддоне
    std::string News = "";              //Признак кантовки
    std::string Top1 = "";              //Отклонения от плоскостности листа До кантовки 1
    std::string Top2 = "";              //Отклонения от плоскостности листа До кантовки 2
    std::string Top3 = "";              //Отклонения от плоскостности листа До кантовки 3
    std::string Top4 = "";              //Отклонения от плоскостности листа До кантовки 4
    std::string Top5 = "";              //Отклонения от плоскостности листа До кантовки 5
    std::string Top6 = "";              //Отклонения от плоскостности листа До кантовки 6
    std::string Top7 = "";              //Отклонения от плоскостности листа До кантовки 7
    std::string Top8 = "";              //Отклонения от плоскостности листа До кантовки 8

    std::string Bot1 = "";              //Отклонения от плоскостности листа После кантовки 1
    std::string Bot2 = "";              //Отклонения от плоскостности листа После кантовки 2
    std::string Bot3 = "";              //Отклонения от плоскостности листа После кантовки 3
    std::string Bot4 = "";              //Отклонения от плоскостности листа После кантовки 4
    std::string Bot5 = "";              //Отклонения от плоскостности листа После кантовки 5
    std::string Bot6 = "";              //Отклонения от плоскостности листа После кантовки 6
    std::string Bot7 = "";              //Отклонения от плоскостности листа После кантовки 7
    std::string Bot8 = "";              //Отклонения от плоскостности листа После кантовки 8

    std::string Hour = "";
    std::string Day = "";               //ID Листа День
    std::string Month = "";             //ID Листа Месяц
    std::string Year = "";              //ID Листа Год
    std::string CassetteNo = "";        //ID Листа Касета
    std::string SheetInCassette = "";   //ID Листа Лист

    std::string TimeForPlateHeat = ""; //Время сигнализации окончания нагрева, мин (Задание Время кончания нагрева)
    std::string PresToStartComp = "";  //Уставка давления для запуска комперссора (Задание давления воды)
    //std::string TempWatTankCool = "";   //Температура закалочной воды для вкл.охлаждения
    std::string Temperature = "";       //Средняя температура 2-части печи закалки (Средняя температура 2-части печи закалки)
    std::string Correct = "";           //Время корректировки
    std::string Pdf = "";               //Путь к файлу PDF
    std::string SecondPos_at = "";      //Время прехода во вторую зону
    std::string InCant_at = "";
    std::string Cant_at = "";
    std::string Delete_at = "";
    TSheet()
    {
    };

    
    bool compare(TSheet& rhs)
    {
        return (Melt == rhs.Melt &&
                PartNo == rhs.PartNo &&
                Pack == rhs.Pack &&
                Sheet == rhs.Sheet);
    }



}TSheet;

namespace casSheet{
    enum cas{
        NN = 0,
        Cassette = 1,
        ID,
        DataTime,
        Start_at, 
        SecondPos_at, 
        DataTime_End,
        //Zone,
#ifdef _DEBUG
        InCant_at,
        Cant_at,
        Correct,
        Diff,
        Pdf,
        Delete_at,
#endif
        Pos,
        News,
        DataTime_All,

        TimeForPlateHeat, //Время сигнализации окончания нагрева, мин
        PresToStartComp,  //Уставка давления для запуска комперссора

        Alloy,
        Thikness,
        Melt,
        Slab,
        PartNo,
        Pack,
        Sheet,
        SubSheet,

        Year,
        Month,
        Day,
        Hour,
        CassetteNo,
        SheetInCassette,


        Temper,
        Temperature, 
        Speed,

        Za_PT3,
        Za_TE3,

        LamPressTop,
        LamPressBot,
        PosClapanTop,
        PosClapanBot,

        Lam1PosClapanTop,
        Lam1PosClapanBot,
        Lam2PosClapanTop,
        Lam2PosClapanBot,

        Lam_TE1,

#ifndef _DEBUG
        Mask,

        Top1,
        Top2,
        Top3,
        Top4,
        Top5,
        Top6,
        Top7,
        Top8,

        Bot1,
        Bot2,
        Bot3,
        Bot4,
        Bot5,
        Bot6,
        Bot7,
        Bot8,
#endif
    };
};

namespace cas{
    enum {
        //DataTime = 0,
        Data = 0,
        Time = 1,
        Alloy = 2,
        Thikness = 3,
        Melt = 4,
        //Slab,
        PartNo = 5,
        Pack = 6,
        Sheet = 7,
        //SubSheet,

        Temper = 8, //Заданная температура в печи закалки, °С
        FactTemp = 9, //Фактическая температура в печи закалки, °С

        TimeForPlateHeat = 10,   //Задание Время окончания нагрева
        DataTime_All = 11,       //Время закалки мин


        //Za_PT3,
        Speed = 12,     //Скорость закалки
        Za_PT3 = 13,    //Давление воды в баке бар
        Za_TE3 = 14,    //Темперратура воды в баке

        //LamPressTop,
        //LamPressBot,
        //PosClapanTop,
        //PosClapanBot,
        //Mask,

        //Lam1PosClapanTop,
        //Lam1PosClapanBot,
        //Lam2PosClapanTop,
        //Lam2PosClapanBot,
        //Lam_TE1,
        
        //News,
        //Top1,
        //Top2,
        //Top3,
        //Top4,
        //Top5,
        //Top6,
        //Top7,
        //Top8,
        //
        //Bot1,
        //Bot2,
        //Bot3,
        //Bot4,
        //Bot5,
        //Bot6,
        //Bot7,
        //Bot8,

        //Day,
        //Month,
        //Year,
        //CassetteNo,
        //SheetInCassette,
        //DataTime_End,
        //DataTime_All,
        //TimeForPlateHeat, //Время сигнализации окончания нагрева, мин
        //PresToStartComp,  //Уставка давления для запуска комперссора
    };
};

extern std::deque<TSheet>AllSheet;
extern HWND SheetWindow;

extern std::string strYear;
extern std::string strMonth;
extern std::string strDay;
extern std::string strHour;
extern std::string strCassetteNo;


void SheetInitApplication();
void SheetInitInstance();

//void FilterIDCasseteSheet(TCassette& p);
//void FilterIDCasseteSheet(int stryear, int strmonth, int strday, int strcassetteno);
void FilterIDCasseteSheet();

//Номер колонки в таблице листов
#pragma region //Номер колонки в таблице листов
extern int Col_Sheet_id;
extern int Col_Sheet_create_at;
extern int Col_Sheet_start_at;
extern int Col_Sheet_datatime_end;
extern int Col_Sheet_pos;
extern int Col_Sheet_datatime_all;
extern int Col_Sheet_alloy;
extern int Col_Sheet_thikness;
extern int Col_Sheet_melt;
extern int Col_Sheet_slab;
extern int Col_Sheet_partno;
extern int Col_Sheet_pack;
extern int Col_Sheet_sheet;
extern int Col_Sheet_subsheet;
extern int Col_Sheet_temper;
extern int Col_Sheet_speed;
extern int Col_Sheet_za_pt3;
extern int Col_Sheet_za_te3;
extern int Col_Sheet_lampresstop;
extern int Col_Sheet_lampressbot;
extern int Col_Sheet_posclapantop ;
extern int Col_Sheet_posclapanbot;
extern int Col_Sheet_mask;
extern int Col_Sheet_lam1posclapantop;
extern int Col_Sheet_lam1posclapanbot;
extern int Col_Sheet_lam2posclapantop;
extern int Col_Sheet_lam2posclapanbot;
extern int Col_Sheet_lam_te1;
extern int Col_Sheet_news;
extern int Col_Sheet_top1;
extern int Col_Sheet_top2;
extern int Col_Sheet_top3;
extern int Col_Sheet_top4;
extern int Col_Sheet_top5;
extern int Col_Sheet_top6;
extern int Col_Sheet_top7;
extern int Col_Sheet_top8;
extern int Col_Sheet_bot1;
extern int Col_Sheet_bot2;
extern int Col_Sheet_bot3;
extern int Col_Sheet_bot4;
extern int Col_Sheet_bot5;
extern int Col_Sheet_bot6;
extern int Col_Sheet_bot7;
extern int Col_Sheet_bot8;
extern int Col_Sheet_day;
extern int Col_Sheet_month;
extern int Col_Sheet_year;
extern int Col_Sheet_cassetteno;
extern int Col_Sheet_sheetincassette;
extern int Col_Sheet_timeforplateheat;
extern int Col_Sheet_prestostartcomp;
extern int Col_Sheet_temperature;
extern int Col_Sheet_pdf;
extern int Col_Sheet_delete_at;
#pragma endregion
