#include "CoreDX.h"

Timer   Core::m_Timer;

bool Core::GameRun()
{
	if (!GameInit())
		return false;
	
	// �߽� ������
	while (MessageProcess())
	{
		if (m_Timer.Frame())
		{
			GameFrame();
			GameRender();
			m_Input.MouseUpdate();		// ���콺 Ȧ��, ���� üũ
		}
	}
	GameRelease();
	
	return true;
}

bool Core::GameFrame() {
	m_Input.Frame();
	Frame();
	return true;
}

bool Core::GameInit()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	if (!m_DxManager.Init() || !Init())
		return false;
	return true;
}

bool Core::GameRender() {
	if (!m_DxManager.PrevRender())	return false;
	
	m_Timer.Render();
	m_Input.Render();
	Render();

	if (!m_DxManager.PostRender())	return false;
	return true;
}


bool Core::GameRelease() {
	Release();
	m_Timer.Release();
	m_Input.Release();
	// ���� ����
	WSACleanup();
	return true;
}


// ������-��ǲ �ڵ鸵
void Core::MsgEvent(const MSG& _message)
{
	m_Input.MsgEvent(_message);
}