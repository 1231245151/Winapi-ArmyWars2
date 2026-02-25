#pragma once

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>	// 메모리 누수 체크

#include <random>
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

#include <iostream>
#include <cwchar>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <string>
using std::string;
using std::wstring;


#include <vector>
#include <list>
#include <map>
using std::vector;
using std::list;
using std::map;
using std::make_pair;


// 라이브러리 링크
#pragma comment(lib, "Msimg32.lib")


extern HINSTANCE g_hInst;
extern HWND hDig;

#include "global.h"