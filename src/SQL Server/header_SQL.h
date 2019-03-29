#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include <string_view>
#include <sstream>	

using namespace std;

#ifdef UNICODE
	#define tstring_view wstring_view
	#define TUCHAR unsigned wchar_t
#else
	#define tstring_view string_view
	#define TUCHAR unsigned char
#endif


//#include <odbcinst.h>
//#pragma comment(lib,"odbc32.lib")
//#pragma comment(lib,"odbcbcp.lib")
//#pragma comment(lib,"OdbcCP32.Lib")