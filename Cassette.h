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
            f_temper,           //���� ����������� �� 5 ����� �� ����� �������
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
        f_temper,           //���� ����������� �� 5 ����� �� ����� �������
        PointTime_1,        //����� �������
        TimeProcSet,        //������ ����� �������� (�������), ���
        PointDTime_2,       //����� ��������
    };
};

typedef struct TCassette{
    std::string Id = "";
    std::string Create_at = "";
    std::string Event = "";
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
    std::string f_temper = "";          //���� ����������� �� 5 ����� �� ����� �������
}TCassette;

namespace cas4{
    enum {
        DataTime = 0,
        Year = 1,
        Month,
        Day,
        CassetteNo,
        SheetInCassette,
        Close,
        Run,
        Error,
        End,
        Delete,
        Peth,
        //TempRef,        //�������� �������� �����������
        PointTime_1,    //����� �������
        PointRef_1,     //������� �����������
        TimeProcSet,    //������ ����� �������� (�������), ���
        PointDTime_2,   //����� ��������
        f_temper,       //���� ����������� �� 5 ����� �� ����� �������

    };
};


extern std::deque<TCassette>AllCassette;
void CassetteInitApplication();
void CassetteInitInstance();