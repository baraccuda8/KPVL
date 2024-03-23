#pragma once

std::string cp1251_to_utf8(std::string str);
std::string utf8_to_cp1251(std::string str);
void SendDebug(std::string cons, std::string ss);
void SendDebug(std::string fun, std::string cons, std::string ss);

//void encode(byte* pText, int len);
std::string GetStringData(std::string d);
char* CmdFileSaveXlsx(HWND hWnd, char* SaveFilename);




