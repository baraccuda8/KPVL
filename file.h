#pragma once

enum class LOGLEVEL{
    LEVEL_INFO = 0,
    LEVEL_WARN = 1,
    LEVEL_ERROR = 2,
};

#define FUNCTION_LINE_NAME (std::string( __FUNCTION__ ) + std::string (":") + std::to_string(__LINE__))
#define __FUN(__s) (FUNCTION_LINE_NAME + "| " + __s).c_str()

#define LOG_INFO(_l, _s2, _c)DebugInfo(LOGLEVEL::LEVEL_INFO, _l, _s2, _c)
#define LOG_WARN(_l, _s2, _c)DebugInfo(LOGLEVEL::LEVEL_WARN, _l, _s2, _c)
#define LOG_ERROR(_l, _s2, _c)DebugInfo(LOGLEVEL::LEVEL_ERROR, _l, _s2, _c)

#define LOG_ERR_SQL(_l, _r, _c){\
LOG_ERROR(_l, FUNCTION_LINE_NAME, _c);\
LOG_ERROR(_l, FUNCTION_LINE_NAME, utf8_to_cp1251(PQresultErrorMessage(_r)));\
}\



#define CATCH(_l, _s) \
    catch(std::filesystem::filesystem_error& exc) { LOG_ERROR(_l, FUNCTION_LINE_NAME, std::string(_s) + ": " + exc.what());} \
    catch(std::runtime_error& exc){LOG_ERROR(_l, FUNCTION_LINE_NAME, std::string(_s) + ": " + exc.what());} \
    catch(std::exception& exc){LOG_ERROR(_l, FUNCTION_LINE_NAME, std::string(_s) + ": " + exc.what());} \
    catch(...){LOG_ERROR(_l, FUNCTION_LINE_NAME, std::string(_s) + ": " + "Unknown error");}


void DebugInfo(LOGLEVEL l, std::string f, std::string s1, std::string s2);

std::string cp1251_to_utf8(std::string str);
std::string utf8_to_cp1251(std::string str);
void SendDebug(std::string cons, std::string ss);
void SendDebug(std::string fun, std::string cons, std::string ss);

//void encode(byte* pText, int len);
//std::string GetStringData(std::string d);

char* CmdFileSaveXlsx(HWND hWnd, char* SaveFilename);
void GetRessusce(int ID, const char* DATE, LPVOID* pLockedResource, DWORD* dwResourceSize);
bool LoadRessurse(std::string fname, int ID);




