// TurmP.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "math.h"

const double PI = atan(1.0) * 4;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TURMP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TURMP);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TURMP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TURMP;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//

//

float GetAngle(POINT ptCenter, POINT ptDest)		//각도 구하기
{
	float fX, fY;
	float fAngle;

	fX = (float)ptDest.x - ptCenter.x;
	fY = (float)ptDest.y - ptCenter.y;

	if(fX == 0.0f)
	{
		fX = 0.000001f;
	}
	fAngle = (float)atan( -fY / fX);
	if(fX < 0)
		fAngle += (float)PI;
	if(ptDest.x >= ptCenter.x && ptDest.y >= ptCenter.y)
		fAngle += 2 * (float)PI;

	return fAngle;
}

struct Triangle
{
	double Width;
	double Height;
};
static Triangle Temp;

void GetPoint(double angle, double hyp)				//각도 계산
{
	static double left_x = 0;
	static double left_y = 0;
	
	Temp.Width;
	Temp.Height;

	Temp.Width = cos(PI/180*angle) * hyp;
	Temp.Height = sin(PI/180*angle) * hyp;

	left_x += Temp.Width - ((int)(Temp.Width*10)/10);
	left_y += Temp.Height - ((int)(Temp.Height*10)/10);

	if(abs((int)left_x) >= 1)
	{
		Temp.Width += ((int)(left_x * 10) / 10);
		left_x -= ((int)(left_x * 10) / 10);
	}

	if(abs((int)left_y) >= 1)
	{
		Temp.Height += ((int)(left_y * 10) / 10);
		left_y -= ((int)(left_y * 10) / 10);
	}

	return;
}


struct Bullet
{
	int left, top;
	int speed;
	int _x, _y;
	bool fire;
};
Bullet BB[200];

struct Enemy
{
	int left, top;
	int speed, life;
	int size, vlife;
	bool enable;
};
Enemy EE[50];

RECT rt;
RECT You;
int You_width;
int You_height;
RECT Object[10];
int Life, Score, Level;
int MoX, MoY;
int g_lParam;
int Ecnt, Delay, bType;
char buff[80];

DWORD WINAPI EnMove(LPVOID lp)			//적 생성
{
	int i;

	for(i = 0; i < 50; i++)
	{
		if(EE[i].enable == false)
		{
			EE[i].enable = true;
			switch(rand() % 8)
			{
			case 0:
				EE[i].left = -50;
				EE[i].top = rt.bottom / 2;
				break;
			case 1:
				EE[i].left = rt.right / 2;
				EE[i].top = -50;
				break;
			case 2:
				EE[i].left = rt.right + 50;
				EE[i].top = rt.bottom / 2;
				break;
			case 3:
				EE[i].left = rt.right / 2;
				EE[i].top = rt.bottom + 50;
				break;
			case 4:
				EE[i].left = -50;
				EE[i].top = -50;
				break;
			case 5:
				EE[i].left = rt.right + 50;
				EE[i].top = rt.top + 50;
				break;
			case 6:
				EE[i].left = -50;
				EE[i].top = rt.bottom + 50;
				break;
			case 7:
				EE[i].left = rt.right + 50;
				EE[i].top = rt.bottom + 50;
				break;
			}
			EE[i].size = rand() % 45 + 25;
			EE[i].speed = rand() % 4 + 2;
			EE[i].life = 6;
			break;
		}
	}

	POINT et, yt;
	for(;;)
	{
		if(EE[i].enable == false || EE[i].life <= 0)
		{
			Score += 500;
			EE[i].enable = false;
			EE[i].vlife = 0;
			break;
		}
		et.x = EE[i].left + EE[i].size / 2;
		et.y = EE[i].top + EE[i].size / 2;
		yt.x = You.left + You_width;
		yt.y = You.top + You_height;
		GetPoint(359 - 57 * GetAngle(et, yt), EE[i].speed);
		int AngX = (int)Temp.Width;
		int AngY = (int)Temp.Height;

		EE[i].left += AngX;
		EE[i].top += AngY;
		Sleep(20);
	}
	
	ExitThread(0);
	return 0;
}


DWORD WINAPI SHOT(LPVOID lp)			//기본, 방사
{
	int x = MoX + LOWORD(lp);
	int y = MoY + HIWORD(lp);
	int i;

	POINT pt, yt;
	pt.x = x + 5;
	pt.y = y + 5;
	yt.x = You.left + You_width;
	yt.y = You.top + You_height;

	float Angle = GetAngle(yt, pt);
	wsprintf(buff, "Angle : %d", (int)(359 - 57 * Angle));

	switch(bType)
	{
	case 0:	
		for(i = 0; i < 100; i++)
		{
			if(BB[i].fire == false)
			{
			BB[i].fire = true;
			BB[i].left = -5;
			BB[i].top = -5;
			BB[i].left += You.left + You_width;
			BB[i].top += You.top + You_height;
			BB[i].speed = 12;
			break;
			}
		}
		GetPoint(359 - 57 * Angle, BB[i].speed);
		Delay = 2;

		break;
	case 3:
		for(i = 0; i < 100; i++)
		{
			if(BB[i].fire == false)
			{
			BB[i].fire = true;
			BB[i].left = -5;
			BB[i].top = -5;
			BB[i].left += You.left + You_width;
			BB[i].top += You.top + You_height;
			BB[i].speed = 6;
			break;
			}
		}
		GetPoint(rand() % 360, BB[i].speed);
		Delay = 0;
		break;
	}

	BB[i]._x = (int)Temp.Width;
	BB[i]._y = (int)Temp.Height;

	for(;;)
	{
		if(BB[i].fire == false)
		{
			break;
		}
		BB[i].left += BB[i]._x;
		BB[i].top += BB[i]._y;
		Sleep(20);
	}

	ExitThread(0);
	return 0;
}


DWORD WINAPI SHOT2(LPVOID lp)					//샷건
{
	int x = MoX + LOWORD(lp);
	int y = MoY + HIWORD(lp);
	int i;
	int c[40];
	int cnt = 0;
	
	POINT pt, yt;
	pt.x = x + 5;
	pt.y = y + 5;
	yt.x = You.left + You_width;
	yt.y = You.top + You_height;

	float Angle = GetAngle(yt, pt);
	wsprintf(buff, "Angle : %d", (int)(359 - 57 * Angle));

		for(i = 0; i < 100; i++)
		{
			if(BB[i].fire == false)
			{
				BB[i].fire = true;
				BB[i].left = -5;
				BB[i].top = -5;
				BB[i].left += You.left + You_width;
				BB[i].top += You.top + You_height;
				BB[i].speed = rand() % 35 + 3;
				GetPoint(rand() % 50 + 334 - 57 * Angle, BB[i].speed);
				BB[i]._x = (int)Temp.Width;
				BB[i]._y = (int)Temp.Height;
				c[cnt] = i;
				cnt++;
			}
			if(cnt >= 40)
				break;
			Delay = 50;
		}

		for(;;)
		{
			if(Delay <= 39)
			{
				for(i = 0; i < 40; i++)
				{
				BB[c[i]].fire = false;
				}
				ExitThread(0);
				return 0;
			}
			for(i = 0; i < 40; i++)
			{
				BB[c[i]].left += BB[c[i]]._x;
				BB[c[i]].top += BB[c[i]]._y;
			}
			Sleep(20);
		}

		ExitThread(0);
		return 0;
}


DWORD WINAPI SHOT3(LPVOID lp)					//박격
{
	int x = MoX + LOWORD(lp);
	int y = MoY + HIWORD(lp);
	int i;
	int c[50];
	int cnt = 0, fir;
	
	POINT pt, yt;
	pt.x = x + 5;
	pt.y = y + 5;
	yt.x = You.left + You_width;
	yt.y = You.top + You_height;

	float Angle = GetAngle(yt, pt);
	wsprintf(buff, "Angle : %d", (int)(359 - 57 * Angle));

		for(i = 0; i < 100; i++)
		{
			if(BB[i].fire == false)
			{
			BB[i].fire = true;
			BB[i].left = -5;
			BB[i].top = -5;
			BB[i].left += You.left + You_width;
			BB[i].top += You.top + You_height;
			BB[i].speed = 20;
			fir = i;
			break;
			}
		}
		GetPoint(359 - 57 * Angle, BB[i].speed);
		Delay = 60;	
		
		BB[i]._x = (int)Temp.Width;
		BB[i]._y = (int)Temp.Height;

		for(;;)
		{
			if(BB[i].fire == false)
			{
				for(i = 0; i < 100; i++)
				{
					if(BB[i].fire == false && fir != i)
					{
						BB[i].left = -5;
						BB[i].top = -5;
						BB[i].left += BB[fir].left + 5;
						BB[i].top += BB[fir].top + 5;
						BB[i].speed = rand() % 20 + 3;
						GetPoint(rand() % 360, BB[i].speed);
						BB[i]._x = (int)Temp.Width;
						BB[i]._y = (int)Temp.Height;
						c[cnt] = i;
						cnt++;
					}
					if(cnt >= 50)
					{
						for(i = 0; i < 50; i++)
						{
							BB[c[i]].fire = true;
							BB[c[i]].left += BB[c[i]]._x;
							BB[c[i]].top += BB[c[i]]._y;
						}
						Delay = 60;
						break;
					}
				}

				for(;;)
				{
					if(Delay <= 45)
					{
						for(i = 0; i < 50; i++)
						{
						BB[c[i]].fire = false;
						}
						ExitThread(0);
						return 0;
					}
					for(i = 0; i < 50; i++)
					{
						BB[c[i]].left += BB[c[i]]._x;
						BB[c[i]].top += BB[c[i]]._y;
					}
					Sleep(20);
				}
			}
			BB[i].left += BB[i]._x;
			BB[i].top += BB[i]._y;
			Sleep(10);
		}

		ExitThread(0);
		return 0;
}


DWORD WINAPI SHOT4(LPVOID lp)				//레이저
{
	int x = MoX + LOWORD(lp);
	int y = MoY + HIWORD(lp);
	int i, cnt = 0;
	int c[200];
	
	POINT pt, yt;
	pt.x = x + 5;
	pt.y = y + 5;
	yt.x = You.left + You_width;
	yt.y = You.top + You_height;
	int __x, __y;

	float Angle = GetAngle(yt, pt);
	wsprintf(buff, "Angle : %d", (int)(359 - 57 * Angle));

	GetPoint(359 - 57 * Angle, 8);
	__x = (int)Temp.Width;
	__y = (int)Temp.Height;

	BB[0].left = -5;
	BB[0].top = -5;
	BB[0].left = You.left + You_width;
	BB[0].top = You.top + You_height;

	for(i = 1; i < 191; i++)
		{
			if(BB[i].fire == false)
			{
				BB[i].fire = true;
				BB[i].left = BB[i-1].left;
				BB[i].top = BB[i-1].top;
				BB[i].left += __x;
				BB[i].top += __y;
				c[cnt] = i;
				cnt++;
			}
			if(cnt >= 190)
				break;
			Delay = 5;
		}


		for(;;)
		{
			if(Delay <= 3)
			{
				for(i = 0; i < 190; i++)
				{
				BB[c[i]].fire = false;
				}
				ExitThread(0);
				return 0;
			}
			for(i = 0; i < 190; i++)
			{
				BB[c[i]].fire = true;
			}
		}

	ExitThread(0);
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HDC BackDC;
	static HDC MapDC;
	static HBITMAP myBit, osBit, myBit2, osBit2;
	HPEN myp, osp;
	HBRUSH myb, osb;
	//HDC memoDC;
	
	char buf[80];
	int i, k, E_CL;
	RECT bRect;
	RECT rEn;

	MoX = You.left - rt.right / 2 + You_width;
	MoY = You.top -rt.bottom / 2 + You_height;

	int wmId, wmEvent;
	PAINTSTRUCT ps;

	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_TIMER:
			if(wParam == 1)
			{
				if(GetAsyncKeyState(65))
				{
					OffsetRect(&You, -5, 0);
				}

				if(GetAsyncKeyState(87))
				{
					OffsetRect(&You, 0, -5);
				}

				if(GetAsyncKeyState(68))
				{
					OffsetRect(&You, 5, 0);
				}

				if(GetAsyncKeyState(83))
				{
					OffsetRect(&You, 0, 5);
				}


				RECT a;

				for(i = 0; i < 50; i++)						//나와 적
				{
					SetRect(&rEn, EE[i].left, EE[i].top, EE[i].left + EE[i].size, EE[i].top + EE[i].size);

					if(EE[i].enable == true && IntersectRect(&a, &You, &rEn))
					{
						Life -= 5;
						int a_W = a.right - a.left;
						int a_H = a.bottom - a.top;

						if(a_W >= a_H)
						{
							if(a.top == You.top)
							{
								OffsetRect(&You, 0, 50);
							}
							if(a.bottom == You.bottom)
							{

								OffsetRect(&You, 0, -50);
							}
						}
						else
						{
							if(a.left == You.left)
							{
								OffsetRect(&You, 50, 0);
							}
							if(a.right == You.right)
							{
								OffsetRect(&You, -50, 0);
							}
						}
					}
				}


				for(i = 0; i < 10; i++)					//나와 장애물
				{
					if(IntersectRect(&a, &You, &Object[i]))
					{
						int a_W = a.right - a.left;
						int a_H = a.bottom - a.top;

						if(a_W >= a_H)
						{
							if(a.top == You.top)
							{
								OffsetRect(&You, 0, a_H);
							}
							if(a.bottom == You.bottom)
							{

								OffsetRect(&You, 0, -a_H);
							}
						}
						else
						{
							if(a.left == You.left)
							{
								OffsetRect(&You, a_W, 0);
							}
							if(a.right == You.right)
							{
								OffsetRect(&You, -a_W, 0);
							}
						}
					}
				}


				for(i = 0; i < 50; i++)					//적과 장애물
				{
					SetRect(&rEn, EE[i].left, EE[i].top, EE[i].left + EE[i].size, EE[i].top + EE[i].size);

					for(k = 0; k < 10; k++)
					{
						if(EE[i].enable == true && IntersectRect(&a, &rEn, &Object[k]))
						{
							int a_W = a.right - a.left;
							int a_H = a.bottom - a.top;

							if(a_W >= a_H)
							{
								if(a.top == rEn.top)
								{
									OffsetRect(&rEn, 0, a_H);
								}
								if(a.bottom == rEn.bottom)
								{

									OffsetRect(&rEn, 0, -a_H);
								}
							}
							else
							{
								if(a.left == rEn.left)
								{
									OffsetRect(&rEn, a_W, 0);
								}
								if(a.right == rEn.right)
								{
									OffsetRect(&rEn, -a_W, 0);
								}
							}
							EE[i].left = rEn.left;
							EE[i].top = rEn.top;
						}
					}
				}


				for(i = 0; i < 200; i++)				//총알과 장애물
				{
					if(BB[i].fire == true)
					{
						SetRect(&bRect, BB[i].left, BB[i].top, BB[i].left + 10, BB[i].top + 10);
						for(k = 0; k < 10; k++)
						{
							if(IntersectRect(&a, &bRect, &Object[k]))
							{
								BB[i].fire = false;
								break;
							}
						}
					}
				}


				for(i = 0; i < 200; i++)				//총알과 적
				{
					if(BB[i].fire == true)
					{
						SetRect(&bRect, BB[i].left, BB[i].top, BB[i].left + 10, BB[i].top + 10);
						for(k = 0; k < 50; k++)
						{
							SetRect(&rEn, EE[k].left, EE[k].top, EE[k].left + EE[k].size, EE[k].top + EE[k].size);
							if(EE[k].enable == true && IntersectRect(&a, &bRect, &rEn) && EE[k].life > 0)
							{
								Score += 100;
								EE[k].life -= 1;
								EE[k].vlife = 200;
								BB[i].fire = false;
								break;
							}
						}
					}
				}


				for(i = 0; i < 200; i++)				//총알과 화면
				{
					SetRect(&bRect, BB[i].left, BB[i].top, BB[i].left + 10, BB[i].top + 10);
					if(BB[i].fire == true && !IntersectRect(&a, &bRect, &rt))
					{
						BB[i].fire = false;
					}
				}


				if(You.left < 0)						//나와 화면
				{
					OffsetRect(&You, -You.left, 0);
				}

				if(You.top < 0)
				{
					OffsetRect(&You, 0, -You.top);
				}

				if(You.right > rt.right)
				{
					OffsetRect(&You, -(You.right - rt.right), 0);
				}

				if(You.bottom > rt.bottom)
				{
					OffsetRect(&You, 0, -(You.bottom - rt.bottom));
				}

				
				if(GetAsyncKeyState(MK_LBUTTON) && Delay <= 0)			//발사
				{
					switch(bType)
					{
					case 0:
						CreateThread(NULL, 0, SHOT, (LPVOID)g_lParam, 0, NULL);
						break;
					case 1:
						CreateThread(NULL, 0, SHOT2, (LPVOID)g_lParam, 0, NULL);
						break;
					case 2:
						CreateThread(NULL, 0, SHOT3, (LPVOID)g_lParam, 0, NULL);
						break;
					case 3:
						CreateThread(NULL, 0, SHOT, (LPVOID)g_lParam, 0, NULL);
						break;
					case 4:
						CreateThread(NULL, 0, SHOT4, (LPVOID)g_lParam, 0, NULL);
						break;
					}
				}
				Delay--;


				if(GetAsyncKeyState(49))					//무기 변경
				{
					bType = 0;
					Delay = 15;
				}else if(GetAsyncKeyState(50))
				{
					bType = 1;
					Delay = 15;
				}else if(GetAsyncKeyState(51))
				{
					bType = 2;
					Delay = 15;
				}else if(GetAsyncKeyState(52))
				{
					bType = 3;
					Delay = 15;
				}else if(GetAsyncKeyState(53))
				{
					for(i = 0; i < 200; i++)
					{
						BB[i].fire = false;
					}
					bType = 4;
					Delay = 15;
				}


				if(Life <= 0)
				{
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					MessageBox(hWnd, "게임 오버!", "ㅈㅈ", MB_OK);
				}


				Ecnt++;
				if(Ecnt > 250 && Level < 20)				//레벨 증가
				{
					Level++;
					Ecnt = 0;
					KillTimer(hWnd, 2);
					SetTimer(hWnd, 2, 2000 - Level * 65, NULL);
				}
			}


			if(wParam == 2)
			{
				CreateThread(NULL, 0, EnMove, (LPVOID)lParam, 0, NULL);
			}

			InvalidateRect(hWnd, NULL, FALSE);

			break;
		case WM_LBUTTONDOWN:
			g_lParam = lParam;
			break;
		case WM_MOUSEMOVE:
			g_lParam = lParam;
			break; 
		case WM_LBUTTONUP:
			break;
		case WM_KEYDOWN:
			break;
		case WM_CREATE:
			hdc = GetDC(hWnd);

			ShowWindow(hWnd, SW_SHOWMAXIMIZED);
			BackDC = CreateCompatibleDC(hdc);
			MapDC = CreateCompatibleDC(hdc);
			
			SetTimer(hWnd, 1, 10, NULL);
			SetTimer(hWnd, 2, 5000, NULL);

			SetRect(&You, 100, 100, 150, 150);		
			You_width = (You.right - You.left) / 2;
			You_height = (You.bottom - You.top) / 2;
			Life = 200;
			Level = 1;

			SetRect(&Object[0], 50, 50, 200, 80);
			SetRect(&Object[1], 300, 150, 500, 200);
			SetRect(&Object[2], 100, 300, 200, 500);
			SetRect(&Object[3], 400, 250, 600, 270);
			SetRect(&Object[4], 500, 350, 600, 400);
			SetRect(&Object[5], 1090, 500, 1350, 550);
			SetRect(&Object[6], 1000, 590, 1050, 850);
			SetRect(&Object[7], 1300, 200, 1500, 400);
			SetRect(&Object[8], 1200, 1000, 1200, 1200);
			SetRect(&Object[9], 200, 700, 600, 800);
			
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			myBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACK));
			osBit = (HBITMAP)SelectObject(BackDC, myBit);
			myBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MAP));
			osBit2 = (HBITMAP)SelectObject(MapDC, myBit2);

			GetClientRect(hWnd, &rt);
			
			Rectangle(MapDC, rt.left, rt.top, rt.right, rt.bottom);
			Rectangle(MapDC, You.left, You.top, You.right, You.bottom);
			
			myb = CreateSolidBrush(RGB(220, 220, 220));
			osb = (HBRUSH)SelectObject(MapDC, myb);
			for(i = 0; i < 10; i++)					//장애물
			{
				Rectangle(MapDC, Object[i].left, Object[i].top, Object[i].right, Object[i].bottom);
			}
			SelectObject(MapDC, osb);
			DeleteObject(myb);

			for(i = 0; i < 200; i++)				//총알
			{
				if(BB[i].fire == true)
				{
					Ellipse(MapDC, BB[i].left, BB[i].top, BB[i].left + 10, BB[i].top + 10);
				}
			}

			for(i = 0; i < 50; i++)					//적
			{
				if(EE[i].enable == true)
				{
					myb = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
					osb = (HBRUSH)SelectObject(MapDC, myb);
					Rectangle(MapDC, EE[i].left, EE[i].top, EE[i].left + EE[i].size, EE[i].top + EE[i].size);
					if(EE[i].vlife > 0)
					{
						EE[i].vlife--;
						SelectObject(MapDC, osb);
						E_CL = EE[i].size - EE[i].size * EE[i].life / 6;
						Rectangle(MapDC, EE[i].left, EE[i].top + EE[i].size + 5, EE[i].left + EE[i].size, EE[i].top + EE[i].size + 10);
						SelectObject(MapDC, myb);
						Rectangle(MapDC, EE[i].left + E_CL, EE[i].top + EE[i].size + 5, EE[i].left + EE[i].size, EE[i].top + EE[i].size + 10);
					}
					SelectObject(MapDC, osb);
					DeleteObject(myb);
				}
			}

			BitBlt(BackDC, 0, 0, rt.right, rt.bottom, MapDC, You.left - rt.right / 2 + You_width, You.top -rt.bottom / 2 + You_height, SRCCOPY);
			
			myb = CreateSolidBrush(RGB(80, 80, 80));
			osb = (HBRUSH)SelectObject(BackDC, myb);
			Rectangle(BackDC, rt.right - 420, 30, rt.right - 20, 55);
			SelectObject(BackDC, osb);
			DeleteObject(myb);

			myb = CreateSolidBrush(RGB(255, 50, 0));
			osb = (HBRUSH)SelectObject(BackDC, myb);
			Rectangle(BackDC, rt.right - 2 * Life - 20, 30, rt.right - 20, 55);
			SelectObject(BackDC, osb);
			DeleteObject(myb);
			
			wsprintf(buf, "Score : %d", Score);
			TextOut(BackDC, rt.right - 140, 70, buf, strlen(buf));

			if(Level == 20)
			{
				wsprintf(buf, "Level : MAX");
			}else
			{
				wsprintf(buf, "Level : %d", Level);
			}
			TextOut(BackDC, rt.right - 140, 90, buf, strlen(buf));

			wsprintf(buf, "x : %d     y : %d", You.left, You.top);
			TextOut(BackDC, rt.right - 140, 110, buf, strlen(buf));

			TextOut(BackDC, rt.right - 140, 130, buff, strlen(buff));
			
			wsprintf(buf, "1 - 연발 2 - 샷건 3 - 박격포 4 - 방사 5 - 레이저", You.left, You.top);
			TextOut(BackDC, rt.right - 330, 160, buf, strlen(buf));

			BitBlt(hdc, 0, 0, rt.right, rt.bottom, BackDC, 0, 0, SRCCOPY);
			DeleteObject(myBit);
			DeleteObject(myBit2);
			DeleteObject(myb);
			DeleteObject(osb);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			DeleteDC(BackDC);
			DeleteDC(MapDC);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
