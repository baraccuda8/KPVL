#include "pch.h"
#include "main.h"
#include "file.h"

std::string cp1251_to_utf8(std::string str)
{
    std::string res;
    std::wstring ures;

    int result_u, result_c;

    result_u = MultiByteToWideChar(1251, 0, &str[0], -1, 0, 0);
    if(!result_u) return "";

    ures.resize(result_u);
    if(!MultiByteToWideChar(1251, 0, &str[0], -1, &ures[0], result_u)) return "";

    result_c = WideCharToMultiByte(CP_UTF8, 0, &ures[0], -1, 0, 0, 0, 0);
    if(!result_c) return "";

    res.resize(result_c);
    if(!WideCharToMultiByte(CP_UTF8, 0, &ures[0], -1, &res[0], result_c, 0, 0)) return "";

    return res;
}

std::string utf8_to_cp1251(std::string str)
{
    if(!str.length()) return "";

    std::wstring res;

    int convertResult;
    convertResult = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    if(convertResult <= 0)
        return "";

    res.resize(convertResult);
    if(MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &res[0], (int)res.size()) <= 0)
        return "";

    convertResult = WideCharToMultiByte(CP_ACP, 0, res.c_str(), (int)res.length(), NULL, 0, NULL, NULL);
    if(convertResult <= 0)
        return "";

    str.resize(convertResult);
    if(WideCharToMultiByte(CP_ACP, 0, res.c_str(), (int)res.length(), &str[0], (int)str.size(), NULL, NULL) <= 0)
        return "";

    return str;
}


void GetRessusce(int ID, const char* DATE, LPVOID* pLockedResource, DWORD* dwResourceSize)
{
    if(!pLockedResource || !dwResourceSize)
        throw std::exception(__FUN("GetRessusce Error Parametr"));

    HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(ID), DATE);
    if(!hResource)
        throw std::exception(__FUN("!FindResource"));

    HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);
    if(!hLoadedResource)
        throw std::exception(__FUN("!hLoadedResource"));

    *pLockedResource = LockResource(hLoadedResource);
    if(!*pLockedResource)
        throw std::exception(__FUN("!*pLockedResource"));

    *dwResourceSize = SizeofResource(hInstance, hResource);
    if(!*dwResourceSize)
        throw std::exception(__FUN("!*dwResourceSize"));

    UnlockResource(hLoadedResource);
}
bool LoadRessurse(std::string fname, int ID)
{
    HGLOBAL hLoadedResource = NULL;
    try
    {

        HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(ID), "DAT");
        if(!hResource)
            throw std::exception("!FindResource");

        hLoadedResource = LoadResource(hInstance, hResource);
        if(!hLoadedResource)
            throw std::exception("!hLoadedResource");

        LPVOID pLockedResource = LockResource(hLoadedResource);
        if(!pLockedResource)
            throw std::exception("!pLockedResource");

        DWORD dwResourceSize = SizeofResource(hInstance, hResource);
        if(!dwResourceSize)
            throw std::exception("!dwResourceSize");

        std::ofstream s(fname, std::ios::binary);
        if(!s.is_open())
            throw std::exception((std::string("Not jpen new filename = \"") + fname + "\"").c_str());

        s.write((char*)pLockedResource, dwResourceSize);
        s.close();

        if(hLoadedResource) FreeResource(hLoadedResource);

        return true;
    }
    catch(std::exception& exc)
    {
        SendDebug(__FUNCTION__, exc.what());
    }
    catch(...)
    {
        SendDebug(__FUNCTION__, "Unknown error");
    };
    if(hLoadedResource) GlobalFree(hLoadedResource);
    return false;

}

BOOL TestFileSave(char* fname)
{
	if(_access(fname, 0) != -1)
	{
		if(MessageBox(NULL, "Файл уже существует.\r\nПерезаписать файл?", "Внимание", MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			while(1)
			{
				std::fstream s(fname, std::fstream::binary | std::fstream::trunc | std::fstream::in | std::fstream::out);
				if(s.is_open())
				{
					s.close();
					return TRUE;
				}
				else
				{
					if(MessageBox(NULL, "Файл открыт в другом редакторе.\r\nЗакройте файл.", "Внимание", MB_RETRYCANCEL | MB_ICONWARNING) == IDCANCEL)
						return FALSE;
					else continue;
				}
			}
		}
		else
			return FALSE;
	}
	return TRUE;
}


char* CmdFileSaveXlsx(HWND hWnd, char* SaveFilename)
{
	OPENFILENAME ofn ={0};

	char filter[] =
		"Microsoft Excel (*.xlsx)\0"
		"*.xlsx\0"
		"All Files\0"
		"*.*\0";

	char filterExt[] = "*.xlsx";
	int nFilterIndex = 0;
    //char SaveFilename[MAX_STRING] = "";
    //strcpy(SaveFilename, name);


	//if(CurrentDirPatch == "")
	//	GetCurrentDirectory(MAX_STRING, cds::szDirName);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInstance;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = nFilterIndex;
	ofn.lpstrFile = SaveFilename;
	ofn.nMaxFile = MAX_STRING;
	ofn.lpstrInitialDir = CurrentDirPatch.c_str();
	ofn.lpstrDefExt = filterExt;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if(GetSaveFileName(&ofn))
	{
		if(TestFileSave(SaveFilename))
			return SaveFilename;
	}
	return NULL;
}

void DebugInfo(LOGLEVEL l, std::string f, std::string s1, std::string s2)
{
    time_t st = time(0);
    std::tm TM;
    localtime_s(&TM, &st);
    char sFormat[1024];
    if(l == LOGLEVEL::LEVEL_INFO)
        sprintf_s(sFormat, 1024, "[%04d-%02d-%02d %02d:%02d:%02d] [INFO] ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);
    else if(l == LOGLEVEL::LEVEL_WARN)
        sprintf_s(sFormat, 1024, "[%04d-%02d-%02d %02d:%02d:%02d] [WARN] ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);
    else if(l == LOGLEVEL::LEVEL_ERROR)
        sprintf_s(sFormat, 1024, "[%04d-%02d-%02d %02d:%02d:%02d] [ERROR] ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);
    else
        sprintf_s(sFormat, 1024, "[%04d-%02d-%02d %02d:%02d:%02d] [CRITICAL] ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);

    std::string file = f + ".log";
    std::ofstream F(file.c_str(), std::ios::binary | std::ios::out | std::ios::app);
    if(F.is_open())
    {
        F << sFormat << s1 << " -> " << s2 << std::endl;
        F.close();
    }
}

void SendDebug(std::string cons, std::string ss)
{
    time_t st = time(0);
    std::tm TM;
    localtime_s(&TM, &st);
    char sFormat[1024];

    sprintf_s(sFormat, 1024, "[%04d-%02d-%02d %02d:%02d:%02d.000] ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);
    std::ofstream F(FullAllDebugLog.c_str(), std::ios::binary | std::ios::out | std::ios::app | std::ios::ate);
    if(F.is_open())
    {
        F << sFormat;
        F << "[" + cons + "] ";
        F << ss << std::endl;
        F.close();
    }
}

void SendDebug(std::string fun, std::string cons, std::string ss)
{
    time_t st = time(0);
    std::tm TM;
    localtime_s(&TM, &st);
    char sFormat[1024];

    sprintf_s(sFormat, 1024, "[%04d-%02d-%02d %02d:%02d:%02d.000] ", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);
    std::ofstream F(FullAllDebugLog.c_str(), std::ios::binary | std::ios::out | std::ios::app | std::ios::ate);
    if(F.is_open())
    {
        F << sFormat;
        F << fun;
        F << " [" + cons + "] ";
        F << ss << std::endl;
        F.close();
    }
}


//std::string GetStringData(std::string d)
//{
//    if(!d.size())return "";
//    std::vector <std::string>split;
//    boost::split(split, d, boost::is_any_of("."), boost::token_compress_on);
//    if(split.size())
//        return split[0];
//    return d;
//}


