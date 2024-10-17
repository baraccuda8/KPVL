// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.
#pragma once


#ifndef NOMINMAX
#define NOMINMAX //prevent windows redefining min/max
#endif 

#include <SDKDDKVer.h>

//#define OPCUACLENT
#define MAX_STRING 1024

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commdlg.h>
#include <fstream>
#include <filesystem>
#include <strsafe.h>
#include <shlobj.h>
#include <stdlib.h>
#include <direct.h>
#include <wtsapi32.h>
#include <psapi.h>
#include <userenv.h>
#include <chrono>
#include <queue>
#include <shellapi.h>
#include <setjmp.h>

#include <gdiplus.h>

#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
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

//#define GDIPLUS;
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#ifdef _WIN64
#pragma comment(lib, "libxl64.lib")
#else
#pragma comment(lib, "libxl32.lib")
#endif


#include "libxl.h"

#include "resource.h"



