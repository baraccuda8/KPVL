#pragma once

#include <libpq-fe.h>
#pragma comment(lib, "libpq.lib")

//#include "resource.h"

extern std::string m_dbhost;
extern std::string m_dbport;
extern std::string m_dbname;
extern std::string m_dbuser;
extern std::string m_dbpass;

std::string cp1251_to_utf8(std::string str);
std::string utf8_to_cp1251(std::string str);

class PGConnection
{
public:
    PGconn* m_connection;
    bool connections;
    PGConnection() { connections = false; };
    ~PGConnection(){ PQfinish(m_connection); };
    bool connection(std::string con);

    PGresult* PGexec(std::string std);

    std::string PGgetvalue(PGresult* res, int l, int i);


private:
    //void establish_connection();
};

typedef struct TTag{
    std::string create_at;
    std::string id;
    std::string id_name;
    std::string name;
    std::string type;
    std::string arhive;
    std::string comment;
    std::string content;
    std::string content_at;
    TTag()
    {
        create_at = "";
        id = "";
        id_name = "";
        name = "";
        type = "";
        arhive = "";
        comment = "";
        content = "";
        content_at = "";
    }
}TTag;

extern std::deque<TTag>AllTag;

bool GetAlloyThicknes(int32_t CodeAlloy, int32_t CodeThicknes, std::string& strAlloy, std::string& strThicknes);


bool InitSQL();
void EndSQL();

extern PGConnection conn_kpvl;
extern PGConnection conn_dops;

