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
            f_temper,           //Факт температуры за 5 минут до конца отпуска
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
        f_temper,           //Факт температуры за 5 минут до конца отпуска
        PointTime_1,        //Время разгона
        TimeProcSet,        //Полное время процесса (уставка), мин
        PointDTime_2,       //Время выдержки
        HeatAcc,           //Факт время нагрева
        HeatWait,          //Факт время выдержки
        Total,             //Факт общее время

    };
};

#define END_AT
enum Cassete{
    NN,
    Id,
    Event,
    Create_at,
#ifdef _DEBUG
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
#ifdef _DEBUG
    End_at,
#endif
    Finish_at,
    Error_at,
#ifdef _DEBUG
    Delete_at,
#endif
    PointRef_1,     //Уставка температуры
    f_temper,       //Факт температуры за 5 минут до конца отпуска

    PointTime_1,    //Время нагрева
    HeatAcc,        //Факт время нагрева
    PointDTime_2,   //Время выдержки
    HeatWait,       //Факт время выдержки
    TimeProcSet,    //Полное время процесса (уставка), мин
    Total,          //Факт общее время
    Correct,
    Pdf
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
    std::string f_temper = "";          //Факт температуры за 5 минут до конца отпуска
    std::string Finish_at = "";         //Завершение процесса + 15 минут
    std::string HeatAcc = "";           //Факт время нагрева
    std::string HeatWait = "";          //Факт время выдержки
    std::string Total = "";             //Факт общее время
    std::string Correct = "";          //Факт время выдержки
    std::string Pdf = "";             //Факт общее время
}TCassette;

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
        Peth,
        //TempRef,      //Заданное значение температуры
        PointTime_1,    //Время разгона
        PointRef_1,     //Уставка температуры
        TimeProcSet,    //Полное время процесса (уставка), мин
        PointDTime_2,   //Время выдержки
        f_temper,       //Факт температуры за 5 минут до конца отпуска

    };
};


extern std::deque<TCassette>AllCassette;
void CassetteInitApplication();
void CassetteInitInstance();
