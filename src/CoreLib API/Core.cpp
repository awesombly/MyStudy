#include "Core.h"




bool Core::GameRun()
{
	if (!GameInit())
		return false;

	// 중심 구동부
	while (MessageProcess())
	{
		if (m_Timer.Frame())
		{
			GameFrame();
			GameRender();
			m_Input.MouseUpdate();		// 마우스 홀드, 프리 체크
		}
	}
	GameRelease();
	
	return true;
}

bool Core::GameInit() {
	m_hScreenDC = GetDC(m_hWnd);							// 메인 버퍼 DC 생성
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);		// 메인 DC와 호환되는 제 2 버퍼 생성
															// 메인 버퍼로부터 호환되는 비트맵 생성
	m_hOffScreenBit = CreateCompatibleBitmap(m_hScreenDC, m_winRect.right, m_winRect.bottom);
	m_hScreenBit = (HBITMAP)SelectObject(m_hOffScreenDC, m_hOffScreenBit);

	if (!Init())
		return false;
	return true;
}

bool Core::GameFrame() {
	m_Input.Frame();
	Frame();
	return true;
}

bool Core::GameRender() {
	m_Timer.Render();
	m_Input.Render();
	Render();

	if (m_Input.isDebug)
	{
		TextOut(m_hOffScreenDC, 10, 10, m_Timer.m_Buffer, (int)wcslen(m_Timer.m_Buffer));
		TextOut(m_hOffScreenDC, 10, 30, m_Input.m_Buffer, (int)wcslen(m_Input.m_Buffer));
		static TCHAR buffer[50] = { 0, };
		_stprintf_s(buffer, L"PlayerCenter ( %d, %d )", (int)MyPlayer::MyCenter.x, (int)MyPlayer::MyCenter.y);
		TextOut(m_hOffScreenDC, 10, 50, buffer, (int)wcslen(buffer));
		//TextOut(m_hOffScreenDC, 10, 90, m_soundManager.m_Buffer, (int)wcslen(m_soundManager.m_Buffer));
		//DrawText(m_HDC, m_Timer.m_Buffer, 10, 0, 0);
	}

	BitBlt(m_hScreenDC, 0, 0, m_winRect.right, m_winRect.bottom, m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
}

bool Core::GameRelease() {
	m_Timer.Release();
	m_Input.Release();
	Release();
	return true;
}

// 윈도우-인풋 핸들링
void Core::MsgEvent(MSG message)
{
	m_Input.MsgEvent(message);
}