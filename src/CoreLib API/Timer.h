#pragma once
#include "Basis.h"
#include "Window.h"


class Timer : public Basis
{
public:
	Timer();
	virtual ~Timer();
public:
	TCHAR	m_Buffer[100];
	static  float	SPF;			// 현재 프레임당 시간
	float	m_AccumulateTime;		// 총 누적 시간
private:
	//const int   FPS;				// 원하는 프레임
	const float m_kDirTick;			// 목표 틱 간격
	int		m_FPS;					// FramePerSecond
	int		m_FrameCnt;				// 프레임 계산 변수
	float	m_ElapseTime;			// 프레임부터 경과 시간
	
	
	DWORD   m_BeforeTick;			// 이전 프레임 틱 저장
	DWORD m_CurrentTick;			// 현재 틱 저장
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
};

