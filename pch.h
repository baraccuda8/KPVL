// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H


#ifndef NOMINMAX
#define NOMINMAX //prevent windows redefining min/max
#endif 

//#define OPCUACLENT
#define MAX_STRING 1024

#include <windows.h>
#include <windowsx.h>

// Добавьте сюда заголовочные файлы для предварительной компиляции

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
//#include <time.h>

#include <conio.h>
#include <stdio.h>
#include <uxtheme.h>
#include <commctrl.h>
#include <commdlg.h>

#include <iostream>
#include <fstream>
#include  <io.h>

#include <mutex>
#include <string>
#include <vector>

#include <set>
#include <exception>
#include <iostream>
#include <filesystem>

#include <iphlpapi.h>
#include <icmpapi.h>

#include <stdexcept>
#include <thread>

#include <gdiplus.h>
//#define GDIPLUS;
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define FUNCTION_LINE_NAME std::string (std::string(" File: ") + std::string( __FILE__ ) + std::string("; Function: ") + std::string( __FUNCTION__ ) + std::string ("; Line ") + std::to_string(__LINE__) + std::string ("; "))


#ifdef _WIN64
#pragma comment(lib, "libxl64.lib")
#else
#pragma comment(lib, "libxl32.lib")
#endif



#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>


using namespace boost::adaptors;

//#include <opc/ua/client/client.h>
//#include <opc/ua/node.h>
//#include <opc/ua/subscription.h>
//#include <opc/common/logger.h>

//#define FUNCTION_LINE_NAME (std::string( __FUNCTION__ ) + std::string (":") + std::to_string(__LINE__))
#define __FUN(__s) (FUNCTION_LINE_NAME + "| " + __s).c_str()

#include "libxl.h"

#include "resource.h"


//Структура заголовков колонок окон ListBox
typedef struct {
    std::string title;
    ULONG cx;
}ListTitle;

#endif //PCH_H

#ifdef _WIN64
#pragma comment(lib, "libxl64.lib")
#else
#pragma comment(lib, "libxl32.lib")
#endif

