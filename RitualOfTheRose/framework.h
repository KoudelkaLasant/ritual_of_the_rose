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
#include <wincodec.h>
using namespace std;
#include <list>
#include <string>
#include <map>
#include <vector>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")
#include "CImg.h"
#include "shellapi.h"
#include "atlstr.h"
#include <chrono>
#include <filesystem>



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif