#pragma once
#include "Input.h"
#include "MyEnemy.h"
#include "ObjectManager.h"


class Core : public Window, public Basis
{
protected:
	Timer	m_Timer;
	Input&	m_Input = Input::GetInstance();
	BitMapManager& m_bitMapManager = BitMapManager::GetInstance();		// 전체 비트맵 관리(Render)
	ObjectManager& m_objectManager = ObjectManager::GetInstance();		// 전체 오브젝트 관리(Memory pull)
	SoundManager&  m_soundManager  = SoundManager::GetInstance();			// 사운드 관리
public:
	Core() = default;
	virtual ~Core() = default;
	virtual bool GameRun()		final;
	virtual bool GameInit()		final;
	virtual bool GameFrame()	final;
	virtual bool GameRender()	final;
	virtual bool GameRelease()	final;
public:
	virtual void MsgEvent(MSG message) override final;
	virtual bool Init()	   = 0;
	virtual bool Frame()   = 0;
	virtual bool Render()  = 0;
	virtual bool Release() = 0;
};