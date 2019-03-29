#include "CoreDX.h"
#include <thread>
#include <mutex>

Timer   Core::m_Timer;
bool Core::isExit = false;

bool Core::GameRun()
{
	if (!GameInit())
		return false;
	
	// 쓰레드 가동
	std::thread gameTimer(&Timer::Frame, &m_Timer);
	std::thread gameFrame(&Core::GameFrame, this);
	std::thread gameRender(&Core::GameRender, this);
	while (MessageProcess())
	{
		//if (m_Timer.Frame())
		//{
//			GameFrame();
	//		GameRender();
			//m_Input.MouseUpdate();		// 마우스 홀드, 프리 체크
		//}
		//std::unique_lock<mutex> lock(m_Timer.m_mutex);
		//m_Timer.m_FrameEvent.wait(lock);
		//m_Timer.m_RenderEvent.wait(lock);
		//
	}
	GameRelease();
	// 모든 쓰레드 종료
	m_ClientServer.TerminateServer();
	isExit = true;
	gameTimer.join();
	gameFrame.join();
	gameRender.join();
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

bool Core::GameFrame() {
	while (!isExit)
	{
		std::unique_lock<mutex> lock(m_Timer.m_mutex);
		m_Timer.m_FrameEvent.wait(lock);

		m_Input.Frame();
		Frame();

		m_Timer.m_RenderEvent.notify_all();
		std::this_thread::yield();
	}
	return true;
}


bool Core::GameRender() {
	while (!isExit)
	{
		static std::mutex renderMutex;
		std::unique_lock<mutex> lock(renderMutex);
		m_Timer.m_RenderEvent.wait(lock);

		if (!m_DxManager.PrevRender())	return false;
		m_Timer.Render();
		m_Input.Render();
		Render();
		if (!m_DxManager.PostRender())	return false;
		m_Input.MouseUpdate();		// 마우스 홀드, 프리 체크

		//std::this_thread::yield();
	}
	return true;
}


bool Core::GameRelease() {
	Release();
	m_Timer.Release();
	m_Input.Release();
	// 접속 종료
	WSACleanup();
	return true;
}


// 윈도우-인풋 핸들링
void Core::MsgEvent(const MSG& _message)
{
	m_Input.MsgEvent(_message);
}