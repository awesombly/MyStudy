#pragma once
#pragma warning(disable : 4005)			// 매크로 재정의 경고 무시
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Ws2tcpip.h>		// InetNtop(), inet_ntop()
#include <WinSock2.h>		// 통신용 헤더
#include <process.h>
#include <stdio.h>
#include <tchar.h>

#include <algorithm>
#include <iostream>
#include <sstream>			// ostringstream
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <ctime>
#include <cmath>
#include <stdexcept>

#pragma comment( lib, "ws2_32.lib")

using namespace std;

namespace Server {
	void	 ErrorMessage(const string_view& msg);
	char*	 WCharToChar(wchar_t* str);
	wchar_t* CharToWChar(char* str);
}