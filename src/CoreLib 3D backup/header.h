#pragma once
#pragma warning(disable : 4005)			// 매크로 재정의 경고 무시
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <Windows.h>
#include <d3d11.h>						// DX11
#include <d3dx11.h>						// 확장 버전
#include <d3dcompiler.h>				// 쉐이더 컴파일 함수
#include <D3DX10math.h>
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
#pragma comment( lib, "CoreLib backup.lib")
#pragma comment( lib, "ServerLib.lib")
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
#pragma comment( lib, "d3d11.lib")		// DX11
#pragma comment( lib, "d3dx10d.lib")	// 벡터, 행렬..
#pragma comment( lib, "d3dx11.lib")		// DX11 Expension
#pragma comment( lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()

constexpr static float PI = 3.14159265358979323846f;
const static D3DXVECTOR3 one = { 0.0f, 0.0f, 0.0f };


using namespace std;

















// 4. 스테이지, 특수객체(색 다르게), 공격마다 밀리는 힘 따로, 보스(무지개색)

// 3. 스크롤	: 상하 벗어나면 이동, wasd 드로우 좌표 기준 조절 + 엎드리면 아래 스크롤(+slerp)













// 메모리 누수
// 객체 스크립트-> 디버그 모드일시 마우스 클릭 드래그로 상태값 변경 : 적용시 객체 위치 찾아서 갱신



// 횡스크롤 or 탑뷰
// 드래그 & 드랍으로 적 집고, 날리고(힘 적용) 충돌 시켜 딜링.. 맵 타일 주어서 던지거나...
// 오른쪽 드래그&드랍 : 해당 방향으로 힘 가함(날라간 객체끼리 또 충돌된다거나.. , 날라가는 중인지:날리는 힘 계산해 연쇄충돌)
// 왼클릭 드래그로 베거나, 스킬 드래그해서 라인, 오브젝트(벽 등) 생성 및 투하
// 객체 클릭후 방향 드래그로 해당 방향 날리기

// 와이어 선 각각을 객체로, 전후 선들과 리스트처럼 연결,
// 와이어 액션(끌당, 이동, 엮기..), 와이어 줄에도 충돌 및 데미지 판정-> 날리거나 교살 등..
// 왼쪽, 오른쪽 각각 할당 : 상하키 입력으로 이동 및 끌당(조이기, 늘리기), 가운데 버튼-엮기(끊기)
// 완전 탄성 충돌(튕기고 튕기고..)