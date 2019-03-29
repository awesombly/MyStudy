#pragma once
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <ctime>
#include <cmath>
#include <sstream>
#include <tchar.h>			// _swprintf_s(TCHAR).. 정의

// 라이브러리 링크를 전처리 해준다
#pragma comment( lib, "CoreLib API.lib")
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()

//#define RUNRUN   int main() { MainClass main;	main.GameRun();	return 0; }

const static float PI = 3.14159265f;

using namespace std;












// AdjustWindowRect(렉트, 스타일, 메뉴유무) : 만드는 스타일에 맞게 클라이언트 영역 맞춤
// GetClientRect() : 클라이언트 영역 얻음
// GetWindowRect() : 윈도우 영역 얻음

// DC(Device Context) - 장치에 그래픽을 출력하는 함수를 호출하기 위한 권한
// GDI(Graphic Device Interface)를 사용할 수 있게 해쥼 : BitBlt, PatBlt...

// getCursorPos()   : 커서 스크린 좌표 반환  ( 모니터 전체 영역에 대한 좌표)
// ScreenToClient() : 스크린->클라이언트 좌표( 해당 윈도우 화면에 대한 좌표)

// TransparentBlt() : 특정 컬러키를 제거하고 뿌림(확,축 가능)
// StrechBlt : 확,축 댐

// _tsplitpath_s(txt, drive, diretory, name, ext)		// 문자의 드라이브, 디렉터리, 이름, 확장자 구별해 저장

// 동적 라이브러리(dll) : 실행 시간에 동적으로 불러옴(exe실행시 포함시켜야 함, need lib)
// 정적 라이브러리(lib) : 실행 파일 자체에 포함시킴(크기 커짐)

// define X(a) #a,  enum 문자열 변환 매크로~




////					윈도우 핸들,         호출한 윈도우 핸들
////  호출형식(언어)  ?                    보여줄 속성값
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//					LPWSTR	  lpCmdLine, int   nCmdShow)
//{
//	// 윈도우 생성
//	MainClass mainWindow;
//
//	mainWindow.SetClientRect(300, 50, 600, 600);
//	// 윈도우 설정
//	if (mainWindow.SetWindow(hInstance, nCmdShow, L"SampleWin"))
//	{
//		// 윈도우 가동
//		mainWindow.GameRun();
//	}
//}