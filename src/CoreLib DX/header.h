#pragma once
#pragma warning(disable : 4005)			// ��ũ�� ������ ��� ����
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <Windows.h>
#include <d3d11.h>						// DX11
#include <d3dx11.h>						// Ȯ�� ����
#include <d3dcompiler.h>				// ���̴� ������ �Լ�
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
#include <tchar.h>						// _swprintf_s(TCHAR).. ����

// ���̺귯�� ��ũ�� ��ó�� ���ش�
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




// CnameApp->OnIdle() ������(Frame, Render, return true��)

// ��������, CnameView->OnSize

// OnBackgroundClolr(), dialog




// 4. ��������, Ư����ü(�� �ٸ���), ���ݸ��� �и��� �� ����, ����(��������)

// 3. ��ũ��	: ���� ����� �̵�, wasd ��ο� ��ǥ ���� ���� + ���帮�� �Ʒ� ��ũ��(+slerp)






// �޸� ����

// ��ü ��ũ��Ʈ-> ����� ����Ͻ� ���콺 Ŭ�� �巡�׷� ���°� ���� : ����� ��ü ��ġ ã�Ƽ� ����