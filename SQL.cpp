#include "pch.h"

#include "main.h"
#include "file.h"
#include "Cassette.h"
#include "Sheet.h"
#include "SQL.h"
#include "LoginDlg.h"

BOOL Run=TRUE;
PGConnection conn_kpvl;
PGConnection conn_dops;
PGConnection conn_tags;


std::string m_dbhost = "localhost";
std::string m_dbport = "5432";
std::string m_dbname = "";
std::string m_dbuser = "";
std::string m_dbpass = "";


extern std::map<int, std::string> EventCassette;
extern std::map<std::string, std::string> NamePos;

std::string PGgetvalue(PGresult* res, int l, int i)
{
    std::string ss = PQgetvalue(res, l, i);
    if(!ss.empty())
        return utf8_to_cp1251(ss);
    else return "";
}

bool PGConnection::�onnection(){
    try
    {
        if(connections)return connections;

        m_connection = PQsetdbLogin(m_dbhost.c_str(), m_dbport.c_str(), NULL, NULL, m_dbname.c_str(), m_dbuser.c_str(), m_dbpass.c_str());

        ConnStatusType st = PQstatus(m_connection);
        if(st != CONNECTION_OK && PQsetnonblocking(m_connection, 1) != 0)
        {
            connections = false;
            throw std::runtime_error(PQerrorMessage(m_connection));
        }

        PGresult* res = PQexec(m_connection, "set time zone 'Asia/Yekaterinburg'");
        ExecStatusType sd = PQresultStatus(res);
        if(sd != PGRES_COMMAND_OK)
        {
            std::string errc = utf8_to_cp1251(PQresultErrorMessage(res));
            throw std::runtime_error(errc);
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
        MessageBox(NULL, "����������� ������", "Error", 0);
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

int PGConnection::PQntuples(PGresult* res)
{
    if(!connections) return 0;
    return ::PQntuples(res);
}


T_Hmi210_1 Hmi210_1;
T_ForBase_RelFurn ForBase_RelFurn_1;
T_ForBase_RelFurn ForBase_RelFurn_2;
T_GenSeqFromHmi GenSeqFromHmi;


const std::string PathKpvl = "|var|PLC210 OPC-UA.Application.";
const std::string AppGenSeqFromHmi = PathKpvl + "GenSeqFromHmi.Data.";


std::deque<Value*> AllTag ={
    {Hmi210_1.Htr1_1  = new Value(PathKpvl + "Hmi210_1.Htr_1.ToHmi.TAct")},       //����������� � ���� 1.1
    {Hmi210_1.Htr1_2  = new Value(PathKpvl + "Hmi210_1.Htr_2.ToHmi.TAct")},       //����������� � ���� 1.2
    {Hmi210_1.Htr1_3  = new Value(PathKpvl + "Hmi210_1.Htr_3.ToHmi.TAct")},       //����������� � ���� 1.3
    {Hmi210_1.Htr1_4  = new Value(PathKpvl + "Hmi210_1.Htr_4.ToHmi.TAct")},       //����������� � ���� 1.4
    {Hmi210_1.Htr2_1 = new Value(PathKpvl + "Hmi210_1.Htr2_1.ToHmi.TAct")},       //����������� � ���� 2.1
    {Hmi210_1.Htr2_2 = new Value(PathKpvl + "Hmi210_1.Htr2_2.ToHmi.TAct")},       //����������� � ���� 2.2
    {Hmi210_1.Htr2_3 = new Value(PathKpvl + "Hmi210_1.Htr2_3.ToHmi.TAct")},       //����������� � ���� 2.3
    {Hmi210_1.Htr2_4 = new Value(PathKpvl + "Hmi210_1.Htr2_4.ToHmi.TAct")},       //����������� � ���� 2.4

    {GenSeqFromHmi.TempSet1     = new Value(AppGenSeqFromHmi + "TempSet1")},       //������� ����������� 

    {AppFurn1.PointTime_1           = new Value(StrFurn1 + "PointTime_1")}, //: REAL; //����� �������
    {AppFurn1.PointRef_1            = new Value(StrFurn1 + "PointRef_1")}, //: REAL;  //������� �����������
    {AppFurn1.PointDTime_2          = new Value(StrFurn1 + "PointDTime_2")}, //: REAL;//����� ��������

    {AppFurn1.ProcRun               = new Value(StrFurn1 + "ProcRun")}, //: BOOL;//������
    {AppFurn1.ProcEnd               = new Value(StrFurn1 + "ProcEnd")}, //: BOOL;//��������� ��������
    {AppFurn1.ProcFault             = new Value(StrFurn1 + "ProcFault")}, //: BOOL;//������ ��������

    {AppFurn1.TimeProcSet           = new Value(StrFurn1 + "TimeProcSet")}, //: REAL;//������ ����� �������� (�������), ���
    {AppFurn1.ProcTimeMin           = new Value(StrFurn1 + "ProcTimeMin")}, //: REAL;//����� ��������, ��� (0..XX)
    {AppFurn1.TimeToProcEnd         = new Value(StrFurn1 + "TimeToProcEnd")}, //: REAL;//����� �� ��������� ��������, ���
    {AppFurn1.TempRef               = new Value(StrFurn1 + "TempRef")}, //: REAL;//�������� �������� �����������
    {AppFurn1.TempAct               = new Value(StrFurn1 + "TempAct")}, //: REAL;//����������� �������� �����������
    {AppFurn1.T1                    = new Value(StrFurn1 + "T1")}, //: REAL; // ��������� 1
    {AppFurn1.T2                    = new Value(StrFurn1 + "T2")}, //: REAL; // ��������� 2
    {AppFurn1.ActTimeHeatAcc        = new Value(StrFurn1 + "ActTimeHeatAcc")}, //: REAL; // ���� ����� �������
    {AppFurn1.ActTimeHeatWait       = new Value(StrFurn1 + "ActTimeHeatWait")}, //: REAL; // ���� ����� ��������
    {AppFurn1.ActTimeTotal          = new Value(StrFurn1 + "ActTimeTotal")}, //: REAL; // ���� ����� �����

    {AppFurn1.Cassette.Day          = new Value(StrFurn1 + "Cassette.Day")}, //ID ������ ����
    {AppFurn1.Cassette.Month        = new Value(StrFurn1 + "Cassette.Month")}, //ID ������ �����
    {AppFurn1.Cassette.Year         = new Value(StrFurn1 + "Cassette.Year")}, //ID ������ ���
    {AppFurn1.Cassette.CassetteNo   = new Value(StrFurn1 + "Cassette.CaasetteNo")}, //ID ������ �����

    //������ ����
    {AppFurn2.PointTime_1           = new Value(StrFurn2 + "PointTime_1")}, //: REAL;//����� �������
    {AppFurn2.PointRef_1            = new Value(StrFurn2 + "PointRef_1")}, //: REAL;//������� �����������
    {AppFurn2.PointDTime_2          = new Value(StrFurn2 + "PointDTime_2")}, //: REAL;//����� ��������

    {AppFurn2.ProcRun               = new Value(StrFurn2 + "ProcRun")}, //: BOOL;//������
    {AppFurn2.ProcEnd               = new Value(StrFurn2 + "ProcEnd")}, //: BOOL;//��������� ��������
    {AppFurn2.ProcFault             = new Value(StrFurn2 + "ProcFault")}, //: BOOL;//������ ��������

    {AppFurn2.TimeProcSet           = new Value(StrFurn2 + "TimeProcSet")}, //: REAL;//������ ����� �������� (�������), ���
    {AppFurn2.ProcTimeMin           = new Value(StrFurn2 + "ProcTimeMin")}, //: REAL;//����� ��������, ��� (0..XX)
    {AppFurn2.TimeToProcEnd         = new Value(StrFurn2 + "TimeToProcEnd")}, //: REAL;//����� �� ��������� ��������, ���
    {AppFurn2.TempRef               = new Value(StrFurn2 + "TempRef")}, //: REAL;//�������� �������� �����������
    {AppFurn2.TempAct               = new Value(StrFurn2 + "TempAct")}, //: REAL;//����������� �������� �����������
    {AppFurn2.T1                    = new Value(StrFurn2 + "T1")}, //: REAL; // ��������� 1
    {AppFurn2.T2                    = new Value(StrFurn2 + "T2")}, //: REAL; // ��������� 2

    {AppFurn2.ActTimeHeatAcc        = new Value(StrFurn2 + "ActTimeHeatAcc")}, //: REAL; // ���� ����� �������
    {AppFurn2.ActTimeHeatWait       = new Value(StrFurn2 + "ActTimeHeatWait")}, //: REAL; // ���� ����� ��������
    {AppFurn2.ActTimeTotal          = new Value(StrFurn2 + "ActTimeTotal")}, //: REAL; // ���� ����� �����

    {AppFurn2.Cassette.Day          = new Value(StrFurn2 + "Cassette.Day")}, ///ID ������ ����
    {AppFurn2.Cassette.Month        = new Value(StrFurn2 + "Cassette.Month")}, //ID ������ �����
    {AppFurn2.Cassette.Year         = new Value(StrFurn2 + "Cassette.Year")},  //ID ������ ���
    {AppFurn2.Cassette.CassetteNo   = new Value(StrFurn2 + "Cassette.CaasetteNo")}, //ID ������ �����
};

bool GetTagTable(std::string patch, PGresult* res, int l)
{
    for(auto& val : AllTag)
    {
        if(val->Name == patch)
        {
            val->ID = conn_kpvl.PGgetvalue(res, l, 0).c_str();
            return true;
        }
    }
    return false;
}

void GetTag()
{
    try
    {
        //Value tag;
        //AllTag.push_back(tag);

        std::string command = "SELECT id, name FROM tag ORDER BY id";
        PGresult* res = conn_kpvl.PGexec(command);
        SendDebug(__FUNCTION__, "conn_kpvl", command);
        if(PQresultStatus(res) == PGRES_TUPLES_OK)
        {
            int line = PQntuples(res);

            for(int l = 0; l < line; l++)
            {
                std::string Patch = conn_kpvl.PGgetvalue(res, l, 1);
                GetTagTable(Patch, res, l);
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



HANDLE hKPVLURI = NULL;
//�����������	���	��������	������	����	������	��������� ����������	�����������
//������	C4996	'std::copy::_Unchecked_iterators::_Deprecate': Call to 'std::copy' with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.To disable this warning, use - D_SCL_SECURE_NO_WARNINGS.See documentation on how to use Visual C++ 'Checked Iterators'	KPVL	C : \Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\xutility	2372

void ItitTag()
{

    GetTag();

    std::string comand;
    PGresult* res;
#pragma region SELECT id, content FROM possheet
    comand = "SELECT id, content FROM possheet"; ///* WHERE name = '" + val->Patch + "'*/;";
    res = conn_kpvl.PGexec(comand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        for(int l = 0; l < line; l++)
            NamePos[conn_kpvl.PGgetvalue(res, l, 0)] = conn_kpvl.PGgetvalue(res, l, 1);
    }
    if(PQresultStatus(res) == PGRES_FATAL_ERROR)
        LOG_ERR_SQL(SQLLogger, res, comand);
    PQclear(res);

    if(!NamePos.size())
    {
        NamePos ={
            {"0", "�� �����" },
            {"1", "1-� ����� ����"},
            {"2", "2-� ����� ����"},
            {"3", "�������"},
            {"4", "����������"},
            {"5", "��������"},
            {"6", "��������"},
            {"7", "� ������"},

            {"10", "������� �� �����" },
            {"11", "������� 1-� ����� ����"},
            {"12", "������� 2-� ����� ����"},
            {"13", "������� �������"},
            {"14", "������� ����������"},
            {"15", "������� ��������"},
            {"16", "������� ��������"},
            {"17", "������� � ������"},

            {"20", "������� �� �����" },
            {"21", "������� 1-� ����� ����"},
            {"22", "������� 2-� ����� ����"},
            {"23", "������� �������"},
            {"24", "������� ����������"},
            {"25", "������� ��������"},
            {"26", "������� ��������"},
            {"27", "������� � ������"},
        };

        std::stringstream ss;
        for(std::map <std::string, std::string>::iterator it = NamePos.begin(); it != NamePos.end(); it++)
        {
            ss << "INSERT INTO possheet (id, content) VALUES ('" << it->first << "', '" << it->second << "');\n";
            NamePos[it->first] = it->second;
        }
        SETUPDATESQL(conn_kpvl, ss);
    }
#pragma endregion

#pragma region SELECT id, content FROM EventCassette
    comand = "SELECT id, content FROM EventCassette"; ///* WHERE name = '" + val->Patch + "'*/;";
    res = conn_kpvl.PGexec(comand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        int line = PQntuples(res);
        for(int l = 0; l < line; l++)
            EventCassette[(evCassete::EV)std::stoi(conn_kpvl.PGgetvalue(res, l, 0))] = conn_kpvl.PGgetvalue(res, l, 1).c_str();
    }
    if(PQresultStatus(res) == PGRES_FATAL_ERROR)
        LOG_ERR_SQL(SQLLogger, res, comand);
    PQclear(res);

    if(!EventCassette.size())
    {
        std::map<int, std::string> eventcassette ={
            {evCassete::Nul,  "����������"},
            {evCassete::Fill, "����������"},
            {evCassete::Wait, "�������"},
            {evCassete::Rel, "������"},
            {evCassete::Error, "������"},
            {evCassete::End, "�����"},
            {evCassete::History, "�� ����"},
            {evCassete::Delete, "������"},
        };

        std::stringstream ss;
        for(std::map <int, std::string>::iterator it = eventcassette.begin(); it != eventcassette.end(); it++)
        {
            ss << "INSERT INTO EventCassette (id, content) VALUES (" << it->first << ", '" << it->second << "');\n";
            EventCassette[it->first] = it->second;
        }
        SETUPDATESQL(conn_kpvl, ss);
        PQclear(res);
    }
#pragma endregion

}

#define TEST
#ifdef TEST
void Test()
{
    std::string comand = "SELECT id, year, month, day, hour, cassetteno FROM cassette WHERE CAST(year AS integer) >= 2024 AND CAST(month AS integer) < 8 ORDER BY id";
    PGresult* res = conn_kpvl.PGexec(comand);
    if(PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        std::string id, year, month, day, hour, cassetteno;
        int line = PQntuples(res);
        for(int l = 0; l < line; l++)
        {
            id = conn_kpvl.PGgetvalue(res, l, 0);
            year = conn_kpvl.PGgetvalue(res, l, 1);
            month = conn_kpvl.PGgetvalue(res, l, 2);
            day = conn_kpvl.PGgetvalue(res, l, 3);
            hour = conn_kpvl.PGgetvalue(res, l, 4);
            cassetteno = conn_kpvl.PGgetvalue(res, l, 5);

            {
                std::stringstream ssd1;
                ssd1 << "UPDATE sheet SET hour = " << hour;
                ssd1 << " WHERE";
                ssd1 << " year = '" << year << "' AND";
                ssd1 << " month = '" << month << "' AND";
                ssd1 << " day = '" << day << "' AND";
                ssd1 << " cassetteno = " << cassetteno << ";";
                SETUPDATESQL(conn_dops, ssd1);
            }

            {
                std::stringstream ssd2;
                ssd2 << "UPDATE cassette SET sheetincassette = ";
                    ssd2 << "(SELECT COUNT(*) FROM sheet";
                    ssd2 << " WHERE";
                    ssd2 << " delete_at IS NULL";
                    ssd2 << " year = '" << year << "' AND";
                    ssd2 << " month = '" << month << "' AND";
                    ssd2 << " day = '" << day << "' AND";
                    ssd2 << " hour = '" << hour << "' AND";
                    ssd2 << " cassetteno = " << cassetteno << ") ";
                ssd2 << " WHERE id = " << id << ";";
                SETUPDATESQL(conn_dops, ssd2);
            }
        }
    }

}
#endif

bool InitSQL()
{
    try
    {
        if(!LoginDlg::LoadConnect())
        {
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, LoginDlg::bagSave);
            if(!conn_kpvl.�onnection())
                throw std::exception("Error SQL connection KPVL");
            if(!conn_dops.�onnection())
                throw std::exception("Error SQL connection dops");
            if(!conn_tags.�onnection())
                throw std::exception("Error SQL connection tags");
            LoginDlg::SaveConnect();
            ItitTag();
        }
        else
        {
            if(!conn_kpvl.�onnection())
                throw std::exception("Error SQL connection KPVL");
            if(!conn_dops.�onnection())
                throw std::exception("Error SQL connection dops");
            if(!conn_tags.�onnection())
                throw std::exception("Error SQL connection tags");
            ItitTag();
#ifdef TEST
            //Test();
#endif
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