#include "Timer.h"
#include "Input.h"
#include "WriteManager.h"
#include "ChattingBox.h"

float Timer::SPF = 0.0f;
int	  Timer::FPS = 0;
float Timer::AccumulateTime = 0.0f;
bool  Timer::isPlaying = true;
Timer*	Timer::Instance = nullptr;

Timer::Timer() : m_kDirTick(1.0f / 60), m_ElapseTime(0.0f), m_FrameCnt(0),
				m_CurrentTick({ 0,0 }), m_BeforeTick({ 0,0 }), m_GameSpeed(1.0f)
{
	Instance = this;
	m_DirFrame = 60;
	Init();
}

Timer::~Timer() {}

bool Timer::Init() noexcept
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_CurrentTick);
	QueryPerformanceCounter(&m_BeforeTick);
	return true;
}

// Ÿ�̸�-������ ó��
bool Timer::Frame() noexcept
{
	// �����ӱ��� ���
	QueryPerformanceCounter(&m_CurrentTick);
	m_ElapseTime = (float)(m_CurrentTick.QuadPart - m_BeforeTick.QuadPart) / m_Frequency.QuadPart;
	
	// �������� ������ ��
	if (m_ElapseTime >= m_kDirTick)
	{
		if (Input::Instance->KeyCheck(VK_BACK) == EKeyState::DOWN)
		{
			if (Input::isChatting && !ChattingBox::m_inputMsg.empty())
				ChattingBox::m_inputMsg.erase(--ChattingBox::m_inputMsg.end());
			else
				isPlaying = !isPlaying;
		}
		if (FPS < m_DirFrame)
			m_kDirTick -= SPF * 0.00001f;
		else if (FPS > m_DirFrame)
			m_kDirTick += SPF * 0.00001f;

     	m_FrameCnt++;
		AccumulateTime += SPF = m_ElapseTime;
		SPF *= m_GameSpeed;

		// 1�ʸ��� ������ ����
		static LARGE_INTEGER prevTick = { 0,0 };
		if ((float)(m_CurrentTick.QuadPart - prevTick.QuadPart) /  m_Frequency.QuadPart >= 1)
		{
			prevTick.QuadPart = m_CurrentTick.QuadPart;
			FPS = m_FrameCnt;
			m_FrameCnt = 0;
			if (Input::isDebug)
			{
				m_InfoStream.str(L"");
				m_InfoStream << L"Time : " << AccumulateTime << L",   FPS : " << FPS << endl;
				OutputDebugString(m_InfoStream.str().c_str());
			}
		}
		m_BeforeTick.QuadPart = m_CurrentTick.QuadPart;
		if (!isPlaying)
			return false;
		return true;
	}
	return false;
}

bool Timer::Render() noexcept
{
	if (Input::isDebug)
	{
		WriteManager::Instance->SetFontSize(5);
		WriteManager::Instance->DrawTextW({ 10, 10, 400, 100 }, m_InfoStream.str());
	}
	return true;
}

void Timer::setDirFrame(const int& dirFrame)
{
	m_DirFrame = dirFrame;
	m_kDirTick = 1.0f / m_DirFrame;
}

void Timer::setGameSpeed(const float& speedRate)
{
	m_GameSpeed = speedRate;
}

bool  Timer::Release() noexcept
{
	return true;
}