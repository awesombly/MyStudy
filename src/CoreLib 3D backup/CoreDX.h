#pragma once
#include "Timer.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "../../include/server/ClientServer.h"

class Core : public Window, public IBasis
{
protected:
	static Timer   m_Timer;
	Input&		   m_Input = Input::GetInstance();
	SoundManager&  m_SoundManager  = SoundManager::GetInstance();	// 荤款靛 包府
	DxManager&	   m_DxManager	   = DxManager::GetInstance();		// DirectX 包府
	ObjectManager& m_ObjectManager = ObjectManager::GetInstance();	// 坷宏璃飘 包府
	WriteManager&  m_WriteManager  = WriteManager::GetInstance();	// 巩磊 免仿 包府
	ClientServer&  m_ClientServer = ClientServer::GetInstance();
public:
	static bool isExit;

	Core() = default;
	virtual ~Core() = default;
	virtual bool GameRun()		final;
	virtual bool GameInit()		final;
	virtual bool GameFrame()	final;
	virtual bool GameRender()	final;
	virtual bool GameRelease()	final;
public:
	virtual void MsgEvent(const MSG& message) override final;
	virtual bool Init()		noexcept = 0;
	virtual bool Frame()	noexcept = 0;
	virtual bool Render()	noexcept = 0;
	virtual bool Release()	noexcept = 0;
};