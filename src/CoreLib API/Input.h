#pragma once
#include "Singleton.h"
#include "Basis.h"
#include "Window.h"

enum class EKeyState : char {
	FREE = 0,
	DOWN,
	UP,
	HOLD
};

class Input : public Singleton<Input>, public Basis
{
private:
	Input();
public:
	friend class Singleton<Input>;
	virtual ~Input();
public:
	TCHAR		 m_Buffer[50];			// ����� ��¿�
	static bool	 isDebug;
private:
	EKeyState	 m_KeyState[256];		
	EKeyState	 m_MouseState[3];
	EKeyState	 m_MousePrevState[3];
	POINT		 m_Cursor;
public:
	POINT		 getCursor();
	EKeyState	 getMouseState(const int& mouseButton);
	EKeyState	 getKeyState(const DWORD& keyValue);
	EKeyState	 KeyCheck(const DWORD& key);			// Ű �˻�(1ȸ��)
	void		 MsgEvent(const MSG& message);			// ���콺 Ŭ��
	void		 MouseUpdate();							// ���콺 ���� ����
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

//#define I_Input  Input::GetInstance()