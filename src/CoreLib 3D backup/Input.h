#pragma once
#include "ISingleton.h"
#include "IBasis.h"
#include "Window.h"

enum class EKeyState : char {
	FREE = 0,
	DOWN,
	UP,
	HOLD
};

class Input : public ISingleton<Input>, public IBasis
{
private:
	Input();
public:
	friend class ISingleton<Input>;
	friend class Timer;
	virtual ~Input();
public:
	//TCHAR		 m_Buffer[50];			// ����� ��¿�
	static bool	 isDebug;
	static bool	 isChatting;
private:
	static POINT m_Cursor;
	EKeyState	 m_KeyState[256];		
	EKeyState	 m_MouseState[3];
	EKeyState	 m_MousePrevState[3];

	EKeyState	 KeyCheck(const DWORD& key);			// Ű �˻�(1ȸ��)
public:
	static POINT		 getCursor();
	EKeyState	 getMouseState(const int& mouseButton);
	EKeyState	 getKeyState(const DWORD& keyValue);
	void		 MsgEvent(const MSG& message);			// ���콺 Ŭ�� ó��
	void		 MouseUpdate();							// ���콺 ���� ����
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
};

//#define I_Input  Input::GetInstance()