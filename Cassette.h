#pragma once

namespace casCassette{
    namespace emCassette{
        enum {
            create_at=0,
            id,
            event,
            day,
            month,
            year,
            cassetteno,
            sheetincassette,
            close_at,
            peth,
            run_at,
            error_at,
            end_at,
            delete_at,
            tempref,            //�������� �������� �����������
            pointtime_1,        //����� �������
            pointref_1,         //������� �����������
            timeprocset,        //������ ����� �������� (�������), ���
            pointdtime_2,       //����� ��������
            facttemper,           //���� ����������� �� 5 ����� �� ����� �������
            HeatAcc,           //���� ����� �������
            HeatWait,          //���� ����� ��������
            Total,             //���� ����� �����

        };
    };

    enum cas {
        nn = 0,
        Id = 1,
        Create_at = 2,
        Year,
        Month,
        Day,
        CassetteNo,
        SheetInCassette,
        Close_at,
        Event,
        Run_at,
        End_at,
        Error_at,
        Delete_at,
        Peth,
        PointRef_1,         //������� �����������
        FactTemper,           //���� ����������� �� 5 ����� �� ����� �������
        PointTime_1,        //����� �������
        TimeProcSet,        //������ ����� �������� (�������), ���
        PointDTime_2,       //����� ��������
        HeatAcc,           //���� ����� �������
        HeatWait,          //���� ����� ��������
        Total,             //���� ����� �����

    };
};
#define _CREATE_AT
#ifdef _DEBUG

    //#define _CLOSE_AT
    #define _END_AT
    #define _DELETE_AT
    //#define _ERROR_AT
    #define _CORRECT
    #define _PDF
    #define _RETURN_AT

#endif // _DEBUG

enum Cassete{
    NN,
    Id,
    Event,
#ifdef _CREATE_AT
    Create_at,
#endif
#ifdef _CLOSE_AT
    Close_at,
#endif
    Year,
    Month,
    Day,
    Hour,
    CassetteNo,
    SheetInCassette,
    Peth,
    Run_at,
#ifdef _END_AT
    End_at,
#endif

    Finish_at,
#ifdef _DELETE_AT
    Delete_at,
#endif
#ifdef _ERROR_AT
    Error_at,
#endif

#ifdef _CORRECT
    Correct,
#endif
#ifdef _PDF
    Pdf,
#endif
#ifdef _RETURN_AT
    Return_at,
#endif

    PointRef_1,     //������� �����������
    FactTemper,       //���� ����������� �� 5 ����� �� ����� �������

    PointTime_1,    //����� �������
    HeatAcc,        //���� ����� �������
    PointDTime_2,   //����� ��������
    HeatWait,       //���� ����� ��������
    TimeProcSet,    //������ ����� �������� (�������), ���
    Total,          //���� ����� �����
};
typedef struct TCassette{
    std::string Id = "";
    std::string Create_at = "";
    std::string Event = "";
    std::string Hour = "";
    std::string Day = "";
    std::string Month = "";
    std::string Year = "";
    std::string CassetteNo = "";
    std::string SheetInCassette = "";
    std::string Close_at = "";
    std::string Peth = "";
    std::string Run_at = "";
    std::string Error_at = "";
    std::string End_at = "";
    std::string Delete_at = "";
    std::string TempRef = "";           //�������� �������� �����������
    std::string PointTime_1 = "";       //����� �������
    std::string PointRef_1 = "";        //������� �����������
    std::string TimeProcSet = "";       //������ ����� �������� (�������), ���
    std::string PointDTime_2 = "";      //����� ��������
    std::string FactTemper = "";          //���� ����������� �� 5 ����� �� ����� �������
    std::string Finish_at = "";         //���������� �������� + 15 �����
    std::string HeatAcc = "";           //���� ����� �������
    std::string HeatWait = "";          //���� ����� ��������
    std::string Total = "";             //���� ����� �����
    std::string Correct = "";          //���� ����� ��������
    std::string Pdf = "";             //���� ����� �����
    std::string Return_at = "";       //������� � ������
	bool isRunAtPref = false;
	bool isEndAtPref = false;
	bool isFinishAtPref = false;
}TCassette;



extern std::deque<TCassette>AllCassette;
void CassetteInitApplication();
void InitListCassette();
void CassetteInitInstance();


namespace cas4{
    enum {
        DataTime = 0,
        Year = 1,
        Month,
        Day,
        Hour,
        CassetteNo,
        SheetInCassette,
        Close,
        Run,
        Error,
        End,
        Delete,
        //Return_at,
        Peth,
        //TempRef,      //�������� �������� �����������
        PointTime_1,    //����� �������
        PointRef_1,     //������� �����������
        TimeProcSet,    //������ ����� �������� (�������), ���
        PointDTime_2,   //����� ��������
        FactTemper,       //���� ����������� �� 5 ����� �� ����� �������

    };
};
