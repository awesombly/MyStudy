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
#include <tchar.h>			// _swprintf_s(TCHAR).. ����

// ���̺귯�� ��ũ�� ��ó�� ���ش�
#pragma comment( lib, "CoreLib API.lib")
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()

//#define RUNRUN   int main() { MainClass main;	main.GameRun();	return 0; }

const static float PI = 3.14159265f;

using namespace std;












// AdjustWindowRect(��Ʈ, ��Ÿ��, �޴�����) : ����� ��Ÿ�Ͽ� �°� Ŭ���̾�Ʈ ���� ����
// GetClientRect() : Ŭ���̾�Ʈ ���� ����
// GetWindowRect() : ������ ���� ����

// DC(Device Context) - ��ġ�� �׷����� ����ϴ� �Լ��� ȣ���ϱ� ���� ����
// GDI(Graphic Device Interface)�� ����� �� �ְ� ���� : BitBlt, PatBlt...

// getCursorPos()   : Ŀ�� ��ũ�� ��ǥ ��ȯ  ( ����� ��ü ������ ���� ��ǥ)
// ScreenToClient() : ��ũ��->Ŭ���̾�Ʈ ��ǥ( �ش� ������ ȭ�鿡 ���� ��ǥ)

// TransparentBlt() : Ư�� �÷�Ű�� �����ϰ� �Ѹ�(Ȯ,�� ����)
// StrechBlt : Ȯ,�� ��

// _tsplitpath_s(txt, drive, diretory, name, ext)		// ������ ����̺�, ���͸�, �̸�, Ȯ���� ������ ����

// ���� ���̺귯��(dll) : ���� �ð��� �������� �ҷ���(exe����� ���Խ��Ѿ� ��, need lib)
// ���� ���̺귯��(lib) : ���� ���� ��ü�� ���Խ�Ŵ(ũ�� Ŀ��)

// define X(a) #a,  enum ���ڿ� ��ȯ ��ũ��~




////					������ �ڵ�,         ȣ���� ������ �ڵ�
////  ȣ������(���)  ?                    ������ �Ӽ���
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//					LPWSTR	  lpCmdLine, int   nCmdShow)
//{
//	// ������ ����
//	MainClass mainWindow;
//
//	mainWindow.SetClientRect(300, 50, 600, 600);
//	// ������ ����
//	if (mainWindow.SetWindow(hInstance, nCmdShow, L"SampleWin"))
//	{
//		// ������ ����
//		mainWindow.GameRun();
//	}
//}