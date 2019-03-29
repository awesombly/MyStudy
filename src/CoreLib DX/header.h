#pragma once
#pragma warning(disable : 4005)			// 매크로 재정의 경고 무시
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <Windows.h>
#include <d3d11.h>						// DX11
#include <d3dx11.h>						// 확장 버전
#include <d3dcompiler.h>				// 쉐이더 컴파일 함수
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
#include <tchar.h>						// _swprintf_s(TCHAR).. 정의

// 라이브러리 링크를 전처리 해준다
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
#pragma comment( lib, "d3d11.lib")		// DX11
#pragma comment( lib, "d3dx11.lib")		// DX11 Expension
#pragma comment( lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "CoreLib DX.lib")
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()
#pragma comment( lib, "ServerLib.lib")

constexpr static float PI = 3.14159265f;

using namespace std;




// CnameApp->OnIdle() 재정의(Frame, Render, return true로)

// 리사이즈, CnameView->OnSize

// OnBackgroundClolr(), dialog




// 4. 스테이지, 특수객체(색 다르게), 공격마다 밀리는 힘 따로, 보스(무지개색)

// 3. 스크롤	: 상하 벗어나면 이동, wasd 드로우 좌표 기준 조절 + 엎드리면 아래 스크롤(+slerp)






// 메모리 누수

// 객체 스크립트-> 디버그 모드일시 마우스 클릭 드래그로 상태값 변경 : 적용시 객체 위치 찾아서 갱신