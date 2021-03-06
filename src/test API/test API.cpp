#include "test API.h"
#include "IntroScene.h"
#include "MainScene.h"
#include "EndingScene.h"

MyBitMap MainClass::m_backGround;
MyBitMap MainClass::m_mapMap;
MainClass* MainClass::m_curScene;
map<wstring, MainClass*> MainClass::m_sceneList;

//void ErrorMessageBox(bool bPrint)
//{
//	TCHAR* lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER |
//		FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, GetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(TCHAR*)&lpMsgBuf, 0, NULL);
//	OutputDebugString((TCHAR*)lpMsgBuf);
//	MessageBox(Window::m_hWnd, lpMsgBuf, _T("에러발생"), MB_OK);
//	LocalFree(lpMsgBuf);
//}

bool MainClass::Init()
{
	m_sceneList[L"Intro"] = new IntroScene();
	m_sceneList[L"Main"]  = new GameScene();
	m_sceneList[L"Ending"] = new EndingScene();
	m_sceneList[L"Intro"]->Init();
	m_curScene = m_sceneList[L"Intro"];

	return true;
}

bool MainClass::Frame()
{
	m_curScene->Frame();

	m_soundManager.Frame();
	if (m_Input.getKeyState(VK_F1) == EKeyState::DOWN)
		m_soundManager.setBGM("BGM/Mischievous Masquerade");
	if (m_Input.getKeyState(VK_F2) == EKeyState::DOWN)
		m_soundManager.setBGM("BGM/Sandbag Mini Game");
	if (m_Input.getKeyState(VK_F3) == EKeyState::DOWN)
		m_soundManager.setBGM("BGM/Brawl Breaks");
	if (m_Input.getKeyState(VK_F4) == EKeyState::DOWN)
		m_soundManager.setBGM("BGM/PLAY ROUGH");

	return true;
}

bool MainClass::Render()
{
	m_curScene->Render();
	return true;
}

bool MainClass::Release()
{
	m_curScene->Release();
	return true;
}

// 씬 전환
void MainClass::setScene(const wstring& sceneName)
{
	m_curScene->Release();
	m_curScene = m_sceneList[sceneName];
	m_curScene->Init();
}


//					윈도우 핸들,         호출한 윈도우 핸들
//  호출형식(언어)  ?                    보여줄 속성값
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPWSTR	  lpCmdLine, int   nCmdShow)
{
	// 윈도우 생성
	MainClass mainWindow;

	mainWindow.SetClientRect(250, 250, 1500, 570);
	// 윈도우 설정
	if (mainWindow.SetWindow(hInstance, nCmdShow, L"MyGame"))
	{
		// 윈도우 가동
		mainWindow.GameRun();	
	}

	lpCmdLine;
	hPrevInstance;
}