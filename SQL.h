#pragma once

#include <libpq-fe.h>
#pragma comment(lib, "libpq.lib")

//#include "resource.h"
#define SQLLogger "SQLLogger"

//#define LOG_ERR_SQL(_l, _r, _c){\
//    SendDebug(_l, _c);\
//    SendDebug(_l, utf8_to_cp1251(PQresultErrorMessage(_r)));\
//}

#define SETUPDATESQL(_c, _s){\
    std::string _comand = _s.str(); \
    PGresult* _res = _c.PGexec(_comand); \
    if(PQresultStatus(_res) == PGRES_FATAL_ERROR)\
        LOG_ERR_SQL(SQLLogger, _res, _comand); \
    PQclear(_res);\
}

#define AppFurn1 ForBase_RelFurn_1
#define AppFurn2 ForBase_RelFurn_2
#define AppCassette cassetteArray.cassette
#define AppSelected1 cassetteArray.selected_cassetFurn1
#define AppSelected2 cassetteArray.selected_cassetFurn2
#define AppCassette1 cassetteArray.cassette[0]
#define AppCassette2 cassetteArray.cassette[1]
#define AppCassette3 cassetteArray.cassette[2]
#define AppCassette4 cassetteArray.cassette[3]
#define AppCassette5 cassetteArray.cassette[4]
#define AppCassette6 cassetteArray.cassette[5]
#define AppCassette7 cassetteArray.cassette[6]

#define StrFurn1 std::string("|var|SPK107 (M01).Application.ForBase_RelFurn_1.Data.")
#define StrFurn2 std::string("|var|SPK107 (M01).Application.ForBase_RelFurn_2.Data.")



extern std::string m_dbhost;
extern std::string m_dbport;
extern std::string m_dbname;
extern std::string m_dbuser;
extern std::string m_dbpass;

std::string cp1251_to_utf8(std::string str);
std::string utf8_to_cp1251(std::string str);


typedef struct Value{
    std::string ID;
    std::string Name;
    Value(): ID(""), Name(""){};
    Value(std::string n): 
        ID(""), 
        Name(n)
    {}
}Value;


typedef struct T_cassette{
    //DINT ����
    Value* Day;

    //DINT �����
    Value* Month;

    //DINT ���
    Value* Year;

    //DINT ����� ������� �� ����
    Value* CassetteNo;

    std::string facttemper = "0";
}T_cassette;

typedef struct T_ForBase_RelFurn{
    time_t Furn_old_dt;

    //BOOL
    Value* WDG_toBase;

    //BOOL
    Value* WDG_fromBase;

    //REAL ����� �������
    Value* PointTime_1;

    //REAL ������� �����������
    Value* PointRef_1;

    //REAL ����� ��������
    Value* PointDTime_2;

    //BOOL ������
    Value* ProcRun;

    //BOOL ��������� ��������
    Value* ProcEnd;

    //BOOL ������ ��������
    Value* ProcFault;

    //REAL ������ ����� �������� (�������), ���
    Value* TimeProcSet;

    //REAL ����� ��������, ��� (0..XX)
    Value* ProcTimeMin;

    //REAL ����� �� ��������� ��������, ���
    Value* TimeToProcEnd;

    //REAL �������� �������� �����������
    Value* TempRef;

    //REAL ����������� �������� �����������
    Value* TempAct;

    //REAL ��������� 1
    Value* T1;

    //REAL ��������� 2
    Value* T2;

    //REAL ���� ����� �������
    Value* ActTimeHeatAcc;

    //REAL ���� ����� ��������
    Value* ActTimeHeatWait;

    //REAL ���� ����� ����� 
    Value* ActTimeTotal;

    T_cassette Cassette;
}T_ForBase_RelFurn;

typedef struct T_Hmi210_1{
    //REAL ����������� � ���� 1.1
    Value* Htr1_1;

    //REAL ����������� � ���� 1.2
    Value* Htr1_2;

    //REAL ����������� � ���� 1.3
    Value* Htr1_3;

    //REAL ����������� � ���� 1.4
    Value* Htr1_4;

    //REAL ����������� � ���� 2.1
    Value* Htr2_1;

    //REAL ����������� � ���� 2.2
    Value* Htr2_2;

    //REAL ����������� � ���� 2.3
    Value* Htr2_3;

    //REAL ����������� � ���� 2.4
    Value* Htr2_4;
}T_Hmi210_1;

typedef struct T_GenSeqFromHmi{
    //REAL ������� ����������� � ����
    Value* TempSet1;
}T_GenSeqFromHmi;

extern T_Hmi210_1 Hmi210_1;
extern T_ForBase_RelFurn ForBase_RelFurn_1;
extern T_ForBase_RelFurn ForBase_RelFurn_2;
extern T_GenSeqFromHmi GenSeqFromHmi;


extern std::deque<Value*> AllTag;

std::string PGgetvalue(PGresult* res, int l, int i);

class PGConnection
{
public:
    PGconn* m_connection = NULL;
    bool connections = false;
    PGConnection() : connections(false) { };
    ~PGConnection(){ PQfinish(m_connection); };
    bool �onnection();

    PGresult* PGexec(std::string std);

    std::string PGgetvalue(PGresult* res, int l, int i);

    int PQntuples(PGresult* res);

private:
    //void establish_connection();
};

//typedef struct TTag{
//    std::string create_at;
//    std::string id;
//    std::string id_name;
//    std::string name;
//    std::string type;
//    std::string arhive;
//    std::string comment;
//    std::string content;
//    std::string content_at;
//    TTag()
//    {
//        create_at = "";
//        id = "";
//        id_name = "";
//        name = "";
//        type = "";
//        arhive = "";
//        comment = "";
//        content = "";
//        content_at = "";
//    }
//}TTag;

//extern std::deque<TTag>AllTag;

bool GetAlloyThicknes(int32_t CodeAlloy, int32_t CodeThicknes, std::string& strAlloy, std::string& strThicknes);


bool InitSQL();
void EndSQL();

namespace evCassete
{
    enum EV{
        Nul = 0, //"����������" },
        Fill = 1, //"����������"},
        Wait = 2, //"�������"},
        Rel = 3, //"������"},
        Error = 4, //"������"},
        End = 5, //"�����"},
        History = 6, //"�����"},
        Delete = 7, //"�������"},
    };
}


extern PGConnection conn_kpvl;
extern PGConnection conn_dops;

void GetColSheet(PGresult* res);

void GetCollCassette(PGresult* res);
void GetCassette(PGresult* res, TCassette& cassette, int line);
