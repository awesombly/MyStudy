#pragma once
#pragma warning(disable : 4005)			// ��ũ�� ������ ��� ����
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <Windows.h>
#include <d3d11.h>						// DX11
#include <d3dx11.h>						// Ȯ�� ����
#include <d3dcompiler.h>				// ���̴� ������ �Լ�
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
#include <tchar.h>						// _swprintf_s(TCHAR).. ����

// ���̺귯�� ��ũ�� ��ó�� ���ش�
#pragma comment( lib, "CoreLib backup.lib")
#pragma comment( lib, "ServerLib.lib")
#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")
#pragma comment( lib, "d3d11.lib")		// DX11
#pragma comment( lib, "d3dx10d.lib")	// ����, ���..
#pragma comment( lib, "d3dx11.lib")		// DX11 Expension
#pragma comment( lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib")		// +timeGetTime()
#pragma comment( lib, "msimg32.lib")	// +TransparentBlt()

constexpr static float PI = 3.14159265358979323846f;
const static D3DXVECTOR3 one = { 0.0f, 0.0f, 0.0f };


using namespace std;

















// 4. ��������, Ư����ü(�� �ٸ���), ���ݸ��� �и��� �� ����, ����(��������)

// 3. ��ũ��	: ���� ����� �̵�, wasd ��ο� ��ǥ ���� ���� + ���帮�� �Ʒ� ��ũ��(+slerp)













// �޸� ����
// ��ü ��ũ��Ʈ-> ����� ����Ͻ� ���콺 Ŭ�� �巡�׷� ���°� ���� : ����� ��ü ��ġ ã�Ƽ� ����



// Ⱦ��ũ�� or ž��
// �巡�� & ������� �� ����, ������(�� ����) �浹 ���� ����.. �� Ÿ�� �־ �����ų�...
// ������ �巡��&��� : �ش� �������� �� ����(���� ��ü���� �� �浹�ȴٰų�.. , ���󰡴� ������:������ �� ����� �����浹)
// ��Ŭ�� �巡�׷� ���ų�, ��ų �巡���ؼ� ����, ������Ʈ(�� ��) ���� �� ����
// ��ü Ŭ���� ���� �巡�׷� �ش� ���� ������

// ���̾� �� ������ ��ü��, ���� ����� ����Ʈó�� ����,
// ���̾� �׼�(����, �̵�, ����..), ���̾� �ٿ��� �浹 �� ������ ����-> �����ų� ���� ��..
// ����, ������ ���� �Ҵ� : ����Ű �Է����� �̵� �� ����(���̱�, �ø���), ��� ��ư-����(����)
// ���� ź�� �浹(ƨ��� ƨ���..)