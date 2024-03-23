#include "pch.h"

#include "main.h"
#include "file.h"
#include "SQL.h"


//UPDATE cassette SET close_at = now() WHERE close_at IS NULL;
//UPDATE cassette SET close_at = DEFAULT WHERE id = 9;
//SELECT create_at, (SELECT name FROM tag WHERE tag.id = todos.id_name), content FROM todos WHERE id_name = 73 OR id_name = 79 ORDER BY create_at DESC;
//SELECT to_char(create_at, 'YYY-MM-DD HH24:MI:SS'), (SELECT name FROM tag WHERE tag.id = todos.id_name), content FROM todos WHERE id_name = 73 OR id_name = 79 ORDER BY create_at DESC;
//SELECT to_char(create_at, 'YYY-MM-DD HH24:MI:SS') AS "Дата Время", (SELECT name FROM tag WHERE tag.id = todos.id_name) AS "Переменная", content AS "Значение" FROM todos WHERE id_name = 73 OR id_name = 79 ORDER BY create_at DESC;

//Функция							Тип результата				Описание													Пример
//to_char(timestamp, text)			text						преобразует время в текст									to_char(current_timestamp, 'HH12:MI:SS')
//to_char(interval, text)			text						преобразует интервал в текст								to_char(interval '15h 2m 12s', 'HH24:MI:SS')
//to_char(int, text)				text						преобразует целое в текст									to_char(125, '999')
//to_char(double precision, text)	text						преобразует плавающее одинарной / двойной точности в текст	to_char(125.8::real, '999D9')
//to_char(numeric, text)			text						преобразует числовое значение в текст						to_char(-125.8, '999D99S')
//to_date(text, text)				date						преобразует текст в дату									to_date('05 Dec 2000', 'DD Mon YYYY')
//to_number(text, text)				numeric						преобразует текст в число									to_number('12,454.8-', '99G999D9S')
//to_timestamp(text, text)			timestamp with time zone	преобразует строку во время									to_timestamp('05 Dec 2000', 'DD Mon YYYY')
//to_timestamp(double precision)	timestamp with time zone	преобразует время в стиле Unix в стандартное время			to_timestamp(1284352323)
//create_at | zone |  id | datatime_end | datatime_all |  alloy | thikness |   melt | partno | pack | sheet | temper | speed | za_pt3 | za_te3 | lampresstop | lampressbot | posclapantop | posclapanbot | mask | lam1posclapantop | lam1posclapanbot | lam2posclapantop | lam2posclapanbot | lam_te1 | news | top1 | top2 | top3 | top4 | top5 | top6 | top7 | top8 | bot1 | bot2 | bot3 | bot4 | bot5 | bot6 | bot7 | bot8 | day | month | year | cassetteno | sheetincassette | pos | 
//SELECT to_char(created, 'YYY-MM-DD HH24:MI:SS') AS "Дата Время", (SELECT name FROM tag WHERE tag.id = todos.id_name) AS "Переменная", content AS "Значение" FROM todos WHERE id_name = 73 OR id_name = 79 ORDER BY created DESC;
//WHERE delete_at IS NULL ORDER BY create_at DESC

BOOL Run=TRUE;
//Соединение с SQL базой
PGConnection conn_kpvl;
PGConnection conn_dops;
PGConnection conn_tags;

//Таблица марок листов
//std::map <int, std::string>MapAlloyCode;

//Таблица толщин листов
//std::map <int, std::map<int, std::string>>MapThicknessCode;

#define SQLFileName "PostgreSQL.dat"

std::string m_dbhost = "192.168.9.61";
std::string m_dbport = "5432";
std::string m_dbname = "kpvl";
std::string m_dbuser = "user";
std::string m_dbpass = "TutonHamon8*";

std::deque<TTag>AllTag;

bool PGConnection::connection(std::string con){
    try
    {
        m_connection = PQsetdbLogin(m_dbhost.c_str(), m_dbport.c_str(), NULL, NULL, m_dbname.c_str(), m_dbuser.c_str(), m_dbpass.c_str());

        if(PQstatus(m_connection) != CONNECTION_OK && PQsetnonblocking(m_connection, 1) != 0)
        {
            connections = false;
            throw std::runtime_error(PQerrorMessage(m_connection));
        }

        PGresult* res = PGexec("SET TIME ZONE 'Asia/Yekaterinburg';");
        SendDebug(con, "SET TIME ZONE 'Asia/Yekaterinburg';");
        if(PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
            SendDebug("conn_kpvl", errc);
        }
        PQclear(res);


        connections = true;
    }
    catch(std::runtime_error rt)
    {
        MessageBox(NULL, rt.what(), "Error", 0);
    }
    catch(...)
    {
        MessageBox(NULL, "Неизвестная ошибка", "Error", 0);
    }
    return connections;
}

PGresult* PGConnection::PGexec(std::string std){
    //SendDebug("<SQL> " + std);
    std::string ss = cp1251_to_utf8(std);
    return
        PQexec(m_connection, ss.c_str());
};

std::string PGConnection::PGgetvalue(PGresult* res, int l, int i)
{
    std::string ss = PQgetvalue(res, l, i);
    if(!ss.empty())
        return utf8_to_cp1251(ss);
    else return "";
}


DLLRESULT InitDialog(HWND hWnd)
{
    CenterWindow(hWnd, NULL);
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT1), m_dbhost.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT2), m_dbport.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT3), m_dbname.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT4), m_dbuser.c_str());
    SetWindowText(GetDlgItem(hWnd, IDC_EDIT5), m_dbpass.c_str());
    return 0;
}

DLLRESULT CommandDialog(HWND hWnd, WPARAM wParam)
{
    if(wParam == IDOK)
    {
        char ss[256];
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT1), ss, 256);    m_dbhost = ss;
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT2), ss, 256);    m_dbport = ss;
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT3), ss, 256);    m_dbname = ss;
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT4), ss, 256);    m_dbuser = ss;
        GetWindowText(GetDlgItem(hWnd, IDC_EDIT5), ss, 256);    m_dbpass = ss;

        if(conn_kpvl.connection("conn_kpvl"))
        {
            //SaveConnect();
            EndDialog(hWnd, FALSE);
        }
    }
    if(wParam == IDCANCEL)
    {
        EndDialog(hWnd, FALSE);
        Quit();
    }
    return 0;
}

DLLRESULT CALLBACK bagSave(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_INITDIALOG)return InitDialog(hWnd);
    if(message == WM_COMMAND) return CommandDialog(hWnd, wParam);
    return (0);
}





void GetTag()
{
    try
    {
        TTag tag;
        AllTag.push_back(tag);

        std::string command = "SELECT * FROM tag ORDER BY id_name";
        PGresult* res = conn_kpvl.PGexec(command);
        SendDebug(__FUNCTION__, "conn_kpvl", command);
        if(PQresultStatus(res) == PGRES_TUPLES_OK)
        {
            int line = PQntuples(res);
            for(int l = 0; l < line; l++)
            {
                tag.create_at = conn_kpvl.PGgetvalue(res, l, 0);
                tag.id =  conn_kpvl.PGgetvalue(res, l, 1);
                tag.id_name = conn_kpvl.PGgetvalue(res, l, 2);
                tag.name = conn_kpvl.PGgetvalue(res, l, 3);
                tag.type = conn_kpvl.PGgetvalue(res, l, 4);
                tag.arhive = conn_kpvl.PGgetvalue(res, l, 5);
                tag.comment = conn_kpvl.PGgetvalue(res, l, 6);
                tag.content = conn_kpvl.PGgetvalue(res, l, 7);
                tag.content_at = conn_kpvl.PGgetvalue(res, l, 8);
                AllTag.push_back(tag);
            }
        }
        else
        {
            std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
            SendDebug("conn_kpvl", errc);
            SendDebug("conn_kpvl", command);
        }
        PQclear(res);
    }
    catch(std::exception& exc)
    {
        WinErrorExit(NULL, exc.what());
    }
    catch(...)
    {
        WinErrorExit(NULL, "Unknown error.");
    }
}


DWORD WINAPI GetTagContent(LPVOID pv)
{
    try
    {
        Run=TRUE;
        GetTag();
        while(Run)
        {
            std::string command = "SELECT id_name, content, create_at FROM tag ORDER BY id_name";
            PGresult* res = conn_tags.PGexec(command);
            //SendDebug("conn_tags", command);
            if(PQresultStatus(res) == PGRES_TUPLES_OK)
            {
                int line = PQntuples(res);
                for(int l = 0; l < line; l++)
                {
                    std::string id_name = conn_kpvl.PGgetvalue(res, l, 0);
                    std::string content = conn_kpvl.PGgetvalue(res, l, 1);
                    std::string content_at = conn_kpvl.PGgetvalue(res, l, 2);
                    for(auto& id : AllTag)
                    {
                        if(id.id_name == id_name)
                        {
                            id.content = content;
                            id.content_at = content_at;
                            break;
                        }
                    }
                    //AllTag.push_back(tag);
                }
            }
            else
            {
                std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
                SendDebug("conn_kpvl", errc);
                SendDebug("conn_kpvl", command);
            }
            PQclear(res);
            int r = 0;
            while(Run && r++ < 10) //10 секунд
                Sleep(1000);
        }
    }
    catch(std::exception& exc)
    {
        WinErrorExit(NULL, exc.what());
    }
    catch(...)
    {
        WinErrorExit(NULL, "Unknown error.");
    }

    return 0;
}

namespace SETALLOY{
    //typedef struct sde{
    //    std::string id = "";
    //    std::string alloy = "";
    //    std::string thikness = "";
    //}sde;
    //std::vector<sde> SDE;


    void GetAlloy()
    {
        //std::string command = "SELECT id, alloy, thikness FROM sheet";
        //PGresult* res = conn_tags.PGexec(command);
        //if(PQresultStatus(res) == PGRES_TUPLES_OK)
        //{
        //    int line = PQntuples(res);
        //    for(int l = 0; l < line; l++)
        //    {
        //        sde s;
        //        s.id = conn_kpvl.PGgetvalue(res, l, 0);;
        //        s.alloy = conn_kpvl.PGgetvalue(res, l, 1);;
        //        s.thikness = conn_kpvl.PGgetvalue(res, l, 2);;
        //        SDE.push_back(s);
        //    }
        //}
        //else
        //{
        //    std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        //    SendDebug("conn_kpvl", errc);
        //    SendDebug("conn_kpvl", command);
        //}
        //PQclear(res);
    }

    void SetAlloy(std::string command)
    {
        //PGresult* res = conn_tags.PGexec(command);
        //if(PQresultStatus(res) == PGRES_TUPLES_OK)
        //{
        //    int line = PQntuples(res);
        //    for(int l = 0; l < line; l++)
        //    {
        //        sde s;
        //        s.id = conn_kpvl.PGgetvalue(res, l, 0);
        //        s.alloy = conn_kpvl.PGgetvalue(res, l, 1);
        //        s.thikness = conn_kpvl.PGgetvalue(res, l, 2);
        //        SDE.push_back(s);
        //    }
        //}
        //else
        //{
        //    std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
        //    SendDebug("conn_kpvl", errc);
        //    SendDebug("conn_kpvl", command);
        //}
        //PQclear(res);
    }

    void Reloc()
    {
        GetAlloy();
        //for(auto s : SDE)
        //{
        //    std::vector <std::string>split_alloy;
        //    std::string DataTime = s.alloy;
        //    boost::split(split_alloy, DataTime, boost::is_any_of(" ("), boost::token_compress_on);
        //    if(split_alloy.size() == 2)
        //        s.alloy = split_alloy[0];

        //    std::vector <std::string>split_thikness;
        //    DataTime = s.thikness;
        //    boost::split(split_thikness, DataTime, boost::is_any_of(" ("), boost::token_compress_on);
        //    if(split_thikness.size() == 2)
        //        s.thikness = split_thikness[0];

        //    std::string command = "UPDATE sheet SET alloy = '" + s.alloy + "', ";
        //    command += "thikness = '" + s.thikness + "'";
        //    command += " WHERE id = " + s.id;
        //    SetAlloy(command);
        //}
    }
}

bool LoadConnect()
{
    char p[1024];
    memset(p, 0, 1024);
    std::ifstream s(SQLFileName, std::ios::binary | std::ios::in);
    if(s.is_open())
    {
        s.read(p, 1024);
        int len = (int)s.gcount();
        s.close();
        //encode((byte*)p, len);
        std::vector <std::string>split;
        boost::split(split, p, boost::is_any_of("\n"));
        if(split.size() == 5)
        {
            m_dbhost = split[0];
            m_dbport = split[1];
            m_dbname = split[2];
            m_dbuser = split[3];
            m_dbpass = split[4];
            return true;
        }
    }
    return false;
}

void SaveConnect()
{
    std::stringstream pass;
    pass << m_dbhost << std::endl
        << m_dbport << std::endl
        << m_dbname << std::endl
        << m_dbuser << std::endl
        << m_dbpass;

    std::string p = pass.str();
    //byte* b = (byte*)&p.c_str()[0];
    //encode((byte*)&p.c_str()[0], (int)p.length());

    std::ofstream s(SQLFileName, std::ios::binary | std::ios::out | std::ios::trunc);
    if(s.is_open())
    {
        s << p;
        s.close();
    }
}

HANDLE hKPVLURI = NULL;
//Серьезность	Код	Описание	Проект	Файл	Строка	Состояние подавления	Подробности
//Ошибка	C4996	'std::copy::_Unchecked_iterators::_Deprecate': Call to 'std::copy' with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.To disable this warning, use - D_SCL_SECURE_NO_WARNINGS.See documentation on how to use Visual C++ 'Checked Iterators'	KPVL	C : \Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\xutility	2372


bool InitSQL()
{
    try
    {
        if(!LoadConnect())
        {
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, bagSave);
            if(!conn_kpvl.connection("conn_kpvl"))
                throw std::exception("Error SQL connection KPVL");
            //Sleep(1000);
            if(!conn_dops.connection("conn_dops"))
                throw std::exception("Error SQL connection dops");
            //Sleep(1000);
            if(!conn_tags.connection("conn_tags"))
                throw std::exception("Error SQL connection tags");
            //Sleep(1000);
            SaveConnect();
            //Sleep(3000);
            //SETALLOY::Reloc();
            hKPVLURI = CreateThread(0, 0, GetTagContent, (LPVOID)0, 0, 0);
        }
        else
        {
            if(!conn_kpvl.connection("conn_kpvl"))
                throw std::exception("Error SQL connection KPVL");
            //Sleep(1000);
            if(!conn_dops.connection("conn_dops"))
                throw std::exception("Error SQL connection dops");
            //Sleep(1000);
            if(!conn_tags.connection("conn_tags"))
                throw std::exception("Error SQL connection tags");
            //SETALLOY::Reloc();
            //Sleep(3000);
            hKPVLURI = CreateThread(0, 0, GetTagContent, (LPVOID)0, 0, 0);
        }
    }
    catch(std::exception& exc)
    {
        WinErrorExit(NULL, exc.what());
    }
    catch(...)
    {
        WinErrorExit(NULL, "Unknown error.");
    }

    return conn_kpvl.connections && conn_dops.connections && conn_tags.connections;
}

void EndSQL()
{
    Run = FALSE;
    WaitForSingleObject(hKPVLURI, INFINITE);
}