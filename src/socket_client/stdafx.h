// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "targetver.h"

#include <Ws2tcpip.h>		// InetNtop(), inet_ntop()
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>		// 통신용 헤더

#include <algorithm>
#include <iostream>
#include <sstream>						// ostringstream
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
#include <process.h>
//#include <pthread.h>

#pragma comment( lib, "ws2_32.lib")

using namespace std;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
