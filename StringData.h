#pragma once

const std::string FORMATTIME1 = "^(\\d{4})-(\\d{2})-(\\d{2}) (\\d{2}):(\\d{2}):(\\d{2})*";
const std::string FORMATTIME2 = "^(\\d{2})-(\\d{2})-(\\d{4}) (\\d{2}):(\\d{2}):(\\d{2})*";

enum class FormatDT: int{
    GMD = 1,
    DMG = 2,
};

inline int Stoi(std::string input)
{
    int out = 0;
    try
    {
        out = std::stoi(input);
    }
    catch(...) {}
    return out;
}

inline float Stof(std::string input)
{
    float out = 0.0f;
    try
    {
        out = std::stof(input);
    }
    catch(...) {}
    return out;
}

inline double Stod(std::string input)
{
    double out = 0.0;
    try
    {
        out = std::stod(input);
    }
    catch(...) {}
    return out;
}
inline std::string string_time(struct tm const* TM)
{
    std::string str(50, '\0');
    sprintf_s(&str[0], 50, "%04d-%02d-%02d %02d:%02d:%02d ", TM->tm_year + 1900, TM->tm_mon + 1, TM->tm_mday, TM->tm_hour, TM->tm_min, TM->tm_sec);
    boost::replace_all(str, "\n", " ");
    return str;
}


std::string GetDataTimeString(FormatDT DT = FormatDT::DMG);

std::string GetDataTimeString(std::time_t& st, FormatDT DT = FormatDT::DMG);

std::string GetDataTimeString(std::time_t* st, FormatDT DT = FormatDT::DMG);

std::string GetDataTimeString(std::tm& st, FormatDT DT = FormatDT::DMG);

time_t DataTimeDiff(std::string str1, std::string str2);
std::string DataTimeDiffString(std::string str1, std::string str2);


time_t DataTimeOfString(std::string str, int& d1, int& d2, int& d3, int& d4, int& d5, int& d6);
time_t DataTimeOfString(std::string str, std::tm& TM);
time_t DataTimeOfString(std::string str);

std::string GetStringData(std::string d);
//std::string Formats(float f, int n = 1);
std::wstring GetData(std::wstring str);

std::string GetDataTimeStr(std::string str);
std::string GetDataTimeStr(std::string str, std::string& outDate, std::string& outTime);
std::string GetDataTimeStr2(std::string str);