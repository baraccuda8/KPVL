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
            tempref,            //Заданное значение температуры
            pointtime_1,        //Время разгона
            pointref_1,         //Уставка температуры
            timeprocset,        //Полное время процесса (уставка), мин
            pointdtime_2,       //Время выдержки
            facttemper,           //Факт температуры за 5 минут до конца отпуска
            HeatAcc,           //Факт время нагрева
            HeatWait,          //Факт время выдержки
            Total,             //Факт общее время

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
        PointRef_1,         //Уставка температуры
        FactTemper,           //Факт температуры за 5 минут до конца отпуска
        PointTime_1,        //Время разгона
        TimeProcSet,        //Полное время процесса (уставка), мин
        PointDTime_2,       //Время выдержки
        HeatAcc,           //Факт время нагрева
        HeatWait,          //Факт время выдержки
        Total,             //Факт общее время

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

    PointRef_1,     //Уставка температуры
    FactTemper,       //Факт температуры за 5 минут до конца отпуска

    PointTime_1,    //Время нагрева
    HeatAcc,        //Факт время нагрева
    PointDTime_2,   //Время выдержки
    HeatWait,       //Факт время выдержки
    TimeProcSet,    //Полное время процесса (уставка), мин
    Total,          //Факт общее время
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
    std::string TempRef = "";           //Заданное значение температуры
    std::string PointTime_1 = "";       //Время разгона
    std::string PointRef_1 = "";        //Уставка температуры
    std::string TimeProcSet = "";       //Полное время процесса (уставка), мин
    std::string PointDTime_2 = "";      //Время выдержки
    std::string FactTemper = "";          //Факт температуры за 5 минут до конца отпуска
    std::string Finish_at = "";         //Завершение процесса + 15 минут
    std::string HeatAcc = "";           //Факт время нагрева
    std::string HeatWait = "";          //Факт время выдержки
    std::string Total = "";             //Факт общее время
    std::string Correct = "";          //Факт время выдержки
    std::string Pdf = "";             //Факт общее время
    std::string Return_at = "";       //Вернули в список
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
        //TempRef,      //Заданное значение температуры
        PointTime_1,    //Время разгона
        PointRef_1,     //Уставка температуры
        TimeProcSet,    //Полное время процесса (уставка), мин
        PointDTime_2,   //Время выдержки
        FactTemper,       //Факт температуры за 5 минут до конца отпуска

    };
};
