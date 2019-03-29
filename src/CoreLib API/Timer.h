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
	static  float	SPF;			// ���� �����Ӵ� �ð�
	float	m_AccumulateTime;		// �� ���� �ð�
private:
	//const int   FPS;				// ���ϴ� ������
	const float m_kDirTick;			// ��ǥ ƽ ����
	int		m_FPS;					// FramePerSecond
	int		m_FrameCnt;				// ������ ��� ����
	float	m_ElapseTime;			// �����Ӻ��� ��� �ð�
	
	
	DWORD   m_BeforeTick;			// ���� ������ ƽ ����
	DWORD m_CurrentTick;			// ���� ƽ ����
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
};

