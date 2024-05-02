#pragma once

#include <libpq-fe.h>
#pragma comment(lib, "libpq.lib")

//#include "resource.h"
#define SQLLogger "SQLLogger"

#define LOG_ERR_SQL(_l, _r, _c)\
{\
    SendDebug(_l, utf8_to_cp1251(PQresultErrorMessage(_r)));\
    SendDebug(_l, _c);\
}

#define SETUPDATESQL(_c, _s) \
{\
    std::string comand = _s.str(); \
    PGresult* res = _c.PGexec(comand); \
    if(PQresultStatus(res) == PGRES_FATAL_ERROR)\
        LOG_ERR_SQL(SQLLogger, res, comand); \
    PQclear(res);\
}



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
    //DINT День
    Value* Day;

    //DINT Месяц
    Value* Month;

    //DINT Год
    Value* Year;

    //DINT Номер кассеты за день
    Value* CassetteNo;

    std::string f_temper = "0";
}T_cassette;

typedef struct T_ForBase_RelFurn{
    time_t Furn_old_dt;

    //BOOL
    Value* WDG_toBase;

    //BOOL
    Value* WDG_fromBase;

    //REAL Время разгона
    Value* PointTime_1;

    //REAL Уставка температуры
    Value* PointRef_1;

    //REAL Время выдержки
    Value* PointDTime_2;

    //BOOL Работа
    Value* ProcRun;

    //BOOL Окончание процесса
    Value* ProcEnd;

    //BOOL Авария процесса
    Value* ProcFault;

    //REAL Полное время процесса (уставка), мин
    Value* TimeProcSet;

    //REAL Время процесса, час (0..XX)
    Value* ProcTimeMin;

    //REAL Время до окончания процесса, мин
    Value* TimeToProcEnd;

    //REAL Заданное значение температуры
    Value* TempRef;

    //REAL Фактическое значение температуры
    Value* TempAct;

    //REAL Термопара 1
    Value* T1;

    //REAL Термопара 2
    Value* T2;

    //REAL Факт время нагрева
    Value* ActTimeHeatAcc;

    //REAL Факт время выдержки
    Value* ActTimeHeatWait;

    //REAL Факт общее время 
    Value* ActTimeTotal;

    T_cassette Cassette;
}T_ForBase_RelFurn;

typedef struct T_Hmi210_1{
    //REAL Температура в зоне 1.1
    Value* Htr1_1;

    //REAL Температура в зоне 1.2
    Value* Htr1_2;

    //REAL Температура в зоне 1.3
    Value* Htr1_3;

    //REAL Температура в зоне 1.4
    Value* Htr1_4;

    //REAL Температура в зоне 2.1
    Value* Htr2_1;

    //REAL Температура в зоне 2.2
    Value* Htr2_2;

    //REAL Температура в зоне 2.3
    Value* Htr2_3;

    //REAL Температура в зоне 2.4
    Value* Htr2_4;
}T_Hmi210_1;

typedef struct T_GenSeqFromHmi{
    //REAL Уставки температуры в печи
    Value* TempSet1;
}T_GenSeqFromHmi;

extern T_Hmi210_1 Hmi210_1;
extern T_ForBase_RelFurn ForBase_RelFurn_1;
extern T_ForBase_RelFurn ForBase_RelFurn_2;
extern T_GenSeqFromHmi GenSeqFromHmi;


extern std::deque<Value*> AllTag;

class PGConnection
{
public:
    PGconn* m_connection;
    bool connections;
    PGConnection() { connections = false; };
    ~PGConnection(){ PQfinish(m_connection); };
    bool connection();

    PGresult* PGexec(std::string std);

    std::string PGgetvalue(PGresult* res, int l, int i);

    int PQntuples(PGresult* res)
    {
        if(!connections) return 0;
        return ::PQntuples(res);
    }

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
        Nul = 0, //"Неизвестно" },
        Fill = 1, //"Набирается"},
        Wait = 2, //"Ожидает"},
        Rel = 3, //"Отпуск"},
        Error = 4, //"Авария"},
        End = 5, //"Конец"},
        History = 6, //"Конец"},
        Delete = 7, //"Удалена"},
    };
}


extern PGConnection conn_kpvl;
extern PGConnection conn_dops;

void GetColSheet(PGresult* res);

void GetCollCassette(PGresult* res);
void GetCassette(PGresult* res, TCassette& cassette, int line);
