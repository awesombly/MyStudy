#include "Timer.h"
#include "Input.h"

float Timer::SPF = 0.0f;

Timer::Timer() : m_kDirTick(1.0f / 100.0f), m_Buffer()
{
	Init();
}

Timer::~Timer() {}

bool Timer::Init()
{
	m_BeforeTick = timeGetTime();			// 윈도우가 시작되고부터의 시간을 틱 단위로 반환
	return true;
}

// 타이머-프레임 처리
bool Timer::Frame()
{
	// 프레임까지 계산
	m_CurrentTick = timeGetTime();
	m_ElapseTime = (m_CurrentTick - m_BeforeTick) / 1000.0f;
	
	// 실질적인 프레임 부
	if (m_ElapseTime >= m_kDirTick)
	{
		m_FrameCnt++;
		m_AccumulateTime += SPF = m_ElapseTime;
		//m_ElapseTime;

		// 1초마다 프레임 갱신
		static DWORD prevTick = 0;
		if (m_CurrentTick - prevTick >= 1000)
		{
			prevTick = m_CurrentTick;
			m_FPS = m_FrameCnt;
			m_FrameCnt = 1;
			if (Input::isDebug)
			{
				//static ostringstream ostr;					// 스트림에 입력함
				//ostr << "진행시간 : " << m_AccumulateTime << endl;
				//OutputDebugStringA(ostr.str().c_str());
			}
		}

		m_BeforeTick = m_CurrentTick;
		return true;
	}
	
	return false;
}

// 프레임 관련 렌더링
bool Timer::Render()
{
	_stprintf_s(m_Buffer, L"FPS : %d,   Time : %.4f,   SPF : %.4f", m_FPS, m_AccumulateTime, SPF);
	return true;
}

bool  Timer::Release()
{
	return true;
}