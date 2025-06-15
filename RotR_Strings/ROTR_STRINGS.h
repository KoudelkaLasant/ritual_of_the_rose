#pragma once
#include "framework.h"

#ifdef ROTR_STRINGS_EXPORTS
#define ROTR_STRINGS_API __declspec(dllexport)
#else
#define ROTR_STRINGS_API __declspec(dllimport)
#endif

extern "C" ROTR_STRINGS_API map<string, map<string, map<string, wstring>>> getStrings();