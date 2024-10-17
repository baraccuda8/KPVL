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
    std::string Cassette = "";          //�� ��������� ������
        
    std::string id = "";                //���������� �����
    std::string DataTime = "";          //����, ����� �������� �����
    std::string Start_at = "";          //����, ����� �������� ����� � ���������� ����
    std::string DataTime_End = "";      //����, ����� ������ ����� �� ���������� ����
    std::string Pos = "";               //������� ������� �����
    std::string DataTime_All = "";      //����������������� �������, ���
    //std::string Zone = "";              //
    std::string Alloy = "";             //����� �����
    std::string Thikness = "";          //������� �����, ��
    std::string Melt = "";              //������
    std::string Slab = "";              //����� �����
    std::string PartNo = "";            //������
    std::string Pack = "";              //�����
    std::string Sheet = "";             //����� �����
    std::string SubSheet = "";          //����� ��������
    std::string Temper = "";            //������� �����������
    std::string Speed = "";             //�������� ��������

    std::string Za_PT3 = "";            //�������� ���� � ����
    std::string Za_TE3 = "";            //����������� ���� � ����

    std::string LaminPressTop = "";     //�������� � ������� ����������
    std::string LaminPressBot = "";     //�������� � ������ ����������
    std::string PosClapanTop = "";      //������. ���������� ������. ����
    std::string PosClapanBot = "";      //������. ���������� ������. ���
    std::string Mask = "";              //����� ������ �������

    std::string Lam1PosClapanTop = "";  //������. ���������� ������ 1. ����
    std::string Lam1PosClapanBot = "";  //������. ���������� ������ 1. ���
    std::string Lam2PosClapanTop = "";  //������. ���������� ������ 2. ����
    std::string Lam2PosClapanBot = "";  //������. ���������� ������ 2. ���

    std::string LAM_TE1 = "";           //����������� ���� � �������
    std::string News = "";              //������� ��������
    std::string Top1 = "";              //���������� �� ������������� ����� �� �������� 1
    std::string Top2 = "";              //���������� �� ������������� ����� �� �������� 2
    std::string Top3 = "";              //���������� �� ������������� ����� �� �������� 3
    std::string Top4 = "";              //���������� �� ������������� ����� �� �������� 4
    std::string Top5 = "";              //���������� �� ������������� ����� �� �������� 5
    std::string Top6 = "";              //���������� �� ������������� ����� �� �������� 6
    std::string Top7 = "";              //���������� �� ������������� ����� �� �������� 7
    std::string Top8 = "";              //���������� �� ������������� ����� �� �������� 8

    std::string Bot1 = "";              //���������� �� ������������� ����� ����� �������� 1
    std::string Bot2 = "";              //���������� �� ������������� ����� ����� �������� 2
    std::string Bot3 = "";              //���������� �� ������������� ����� ����� �������� 3
    std::string Bot4 = "";              //���������� �� ������������� ����� ����� �������� 4
    std::string Bot5 = "";              //���������� �� ������������� ����� ����� �������� 5
    std::string Bot6 = "";              //���������� �� ������������� ����� ����� �������� 6
    std::string Bot7 = "";              //���������� �� ������������� ����� ����� �������� 7
    std::string Bot8 = "";              //���������� �� ������������� ����� ����� �������� 8

    std::string Hour = "";
    std::string Day = "";               //ID ����� ����
    std::string Month = "";             //ID ����� �����
    std::string Year = "";              //ID ����� ���
    std::string CassetteNo = "";        //ID ����� ������
    std::string SheetInCassette = "";   //ID ����� ����

    std::string TimeForPlateHeat = ""; //����� ������������ ��������� �������, ��� (������� ����� �������� �������)
    std::string PresToStartComp = "";  //������� �������� ��� ������� ����������� (������� �������� ����)
    //std::string TempWatTankCool = "";   //����������� ���������� ���� ��� ���.����������
    std::string Temperature = "";       //������� ����������� 2-����� ���� ������� (������� ����������� 2-����� ���� �������)
    std::string Correct = "";           //����� �������������
    std::string Pdf = "";               //���� � ����� PDF
    std::string SecondPos_at = "";      //����� ������� �� ������ ����
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

        TimeForPlateHeat, //����� ������������ ��������� �������, ���
        PresToStartComp,  //������� �������� ��� ������� �����������

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

        Temper = 8, //�������� ����������� � ���� �������, ��
        FactTemp = 9, //����������� ����������� � ���� �������, ��

        TimeForPlateHeat = 10,   //������� ����� ��������� �������
        DataTime_All = 11,       //����� ������� ���


        //Za_PT3,
        Speed = 12,     //�������� �������
        Za_PT3 = 13,    //�������� ���� � ���� ���
        Za_TE3 = 14,    //������������ ���� � ����

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
        //TimeForPlateHeat, //����� ������������ ��������� �������, ���
        //PresToStartComp,  //������� �������� ��� ������� �����������
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

//����� ������� � ������� ������
#pragma region //����� ������� � ������� ������
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
