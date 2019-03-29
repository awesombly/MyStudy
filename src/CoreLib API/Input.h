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
	TCHAR		 m_Buffer[50];			// 디버그 출력용
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
	EKeyState	 KeyCheck(const DWORD& key);			// 키 검사(1회만)
	void		 MsgEvent(const MSG& message);			// 마우스 클릭
	void		 MouseUpdate();							// 마우스 상태 갱신
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

//#define I_Input  Input::GetInstance()