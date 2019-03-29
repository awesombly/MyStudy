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
	m_BeforeTick = timeGetTime();			// �����찡 ���۵ǰ������ �ð��� ƽ ������ ��ȯ
	return true;
}

// Ÿ�̸�-������ ó��
bool Timer::Frame()
{
	// �����ӱ��� ���
	m_CurrentTick = timeGetTime();
	m_ElapseTime = (m_CurrentTick - m_BeforeTick) / 1000.0f;
	
	// �������� ������ ��
	if (m_ElapseTime >= m_kDirTick)
	{
		m_FrameCnt++;
		m_AccumulateTime += SPF = m_ElapseTime;
		//m_ElapseTime;

		// 1�ʸ��� ������ ����
		static DWORD prevTick = 0;
		if (m_CurrentTick - prevTick >= 1000)
		{
			prevTick = m_CurrentTick;
			m_FPS = m_FrameCnt;
			m_FrameCnt = 1;
			if (Input::isDebug)
			{
				//static ostringstream ostr;					// ��Ʈ���� �Է���
				//ostr << "����ð� : " << m_AccumulateTime << endl;
				//OutputDebugStringA(ostr.str().c_str());
			}
		}

		m_BeforeTick = m_CurrentTick;
		return true;
	}
	
	return false;
}

// ������ ���� ������
bool Timer::Render()
{
	_stprintf_s(m_Buffer, L"FPS : %d,   Time : %.4f,   SPF : %.4f", m_FPS, m_AccumulateTime, SPF);
	return true;
}

bool  Timer::Release()
{
	return true;
}