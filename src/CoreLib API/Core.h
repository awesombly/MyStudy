#pragma once
#include "Input.h"
#include "MyEnemy.h"
#include "ObjectManager.h"


class Core : public Window, public Basis
{
protected:
	Timer	m_Timer;
	Input&	m_Input = Input::GetInstance();
	BitMapManager& m_bitMapManager = BitMapManager::GetInstance();		// ��ü ��Ʈ�� ����(Render)
	ObjectManager& m_objectManager = ObjectManager::GetInstance();		// ��ü ������Ʈ ����(Memory pull)
	SoundManager&  m_soundManager  = SoundManager::GetInstance();			// ���� ����
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