// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <wincodec.h>
using namespace std;
#include <list>
#include <string>
#include <map>
#include <vector>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "soloud_static.lib")
#include "CImg.h"
#include "shellapi.h"
#include "atlstr.h"
#include <chrono>
#include <filesystem>
#include <thread>
#include "Mmsystem.h"
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_fader.h"
#include <mutex>
#include "json.hpp"
using json = nlohmann::json;
#include <fstream>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#include <iostream>
#include <format>
#include "ROTR_STRINGS.h"


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

typedef map<string, map<string, map<string, wstring>>>(__stdcall * f_funci)();
filesystem::path path = filesystem::current_path() / "ROTR_Strings.dll";
wstring pathToDLL = path.c_str();

HINSTANCE hGetProcIDDLL = LoadLibrary(pathToDLL.c_str());
f_funci getStrings_call = (f_funci)GetProcAddress(hGetProcIDDLL, "getStrings");

map<string, map<string, map<string, wstring>>> strings = getStrings_call();