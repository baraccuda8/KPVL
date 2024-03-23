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

    std::string id = "";
    std::string DataTime = "";
    std::string Start_at = "";
    std::string DataTime_End = "";
    std::string DataTime_All = "";
    std::string Zone = "";
    std::string Alloy = "";
    std::string Thikness = "";
    std::string Melt = "";
    std::string Slab = "";
    std::string PartNo = "";
    std::string Pack = "";
    std::string Sheet = "";
    std::string SubSheet = "";
    std::string Temper = "";
    std::string Speed = "";

    std::string Za_PT3 = "";
    std::string Za_TE3 = "";

    std::string LaminPressTop = "";
    std::string LaminPressBot = "";
    std::string PosClapanTop = "";
    std::string PosClapanBot = "";
    std::string Mask = "";

    std::string Lam1PosClapanTop = "";
    std::string Lam1PosClapanBot = "";
    std::string Lam2PosClapanTop = "";
    std::string Lam2PosClapanBot = "";

    std::string LAM_TE1 = "";
    std::string News = "";
    std::string Top1 = "";
    std::string Top2 = "";
    std::string Top3 = "";
    std::string Top4 = "";
    std::string Top5 = "";
    std::string Top6 = "";
    std::string Top7 = "";
    std::string Top8 = "";

    std::string Bot1 = "";
    std::string Bot2 = "";
    std::string Bot3 = "";
    std::string Bot4 = "";
    std::string Bot5 = "";
    std::string Bot6 = "";
    std::string Bot7 = "";
    std::string Bot8 = "";

    std::string Year = "";
    std::string Month = "";
    std::string Day = "";
    std::string CassetteNo = "";
    std::string SheetInCassette = "";
    std::string Pos = "";

    std::string TimeForPlateHeat = ""; //Время сигнализации окончания нагрева, мин
    std::string PresToStartComp = "";  //Уставка давления для запуска комперссора
    //std::string TempWatTankCool = "";  //Температура закалочной воды для вкл.охлаждения

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
        ID = 1,
        DataTime = 2,
        Start_at, 
        //Zone,
        Pos,
        News,
        DataTime_End,
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


        Temper,
        Speed,

        Za_PT3,
        Za_TE3,

        LamPressTop,
        LamPressBot,
        PosClapanTop,
        PosClapanBot,
        Mask,

        Lam1PosClapanTop,
        Lam1PosClapanBot,
        Lam2PosClapanTop,
        Lam2PosClapanBot,

        Lam_TE1,

        Year,
        Month,
        Day,
        CassetteNo,
        SheetInCassette,

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
    };
};

namespace cas{
    enum {
        DataTime = 0,
        Alloy = 1,
        Thikness = 2,
        Melt = 3,
        Slab,
        PartNo,
        Pack,
        Sheet,
        SubSheet,

        Temper,
        Speed,

        Za_PT3,
        Za_TE3,

        LamPressTop,
        LamPressBot,
        PosClapanTop,
        PosClapanBot,
        Mask,

        Lam1PosClapanTop,
        Lam1PosClapanBot,
        Lam2PosClapanTop,
        Lam2PosClapanBot,

        Lam_TE1,
        News,
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

        Day,
        Month,
        Year,
        CassetteNo,
        SheetInCassette,
        DataTime_End,
        DataTime_All,
        TimeForPlateHeat, //Время сигнализации окончания нагрева, мин
        PresToStartComp,  //Уставка давления для запуска комперссора
    };
};

extern std::deque<TSheet>AllSheet;
extern HWND SheetWindow;

void SheetInitApplication();
void SheetInitInstance();

//void FilterIDCasseteSheet(TCassette& p);
void FilterIDCasseteSheet(int stryear, int strmonth, int strday, int strcassetteno);
