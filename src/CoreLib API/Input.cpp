#include "Input.h"

bool Input::isDebug = false;

Input::Input() : m_KeyState(), m_MouseState(), m_MousePrevState(), m_Buffer()
{
	Init();
}

Input::~Input() {}

EKeyState Input::KeyCheck(const DWORD& keyValue)
{
	// GetKeyState()		// �����
	// GetAsyncKeyState()	// �񵿱��
	short sKey = GetAsyncKeyState(keyValue);
	// Ŭ�� �����Ͻ� �ֻ��� ��Ʈ�� 1�� �ٲ���
	if (sKey & 0x8000)		// ����			
	{
		if (m_KeyState[keyValue] == EKeyState::FREE)
			return m_KeyState[keyValue] = EKeyState::DOWN;
		else
			return m_KeyState[keyValue] = EKeyState::HOLD;
	}
	else                    // �ȴ���
	{
		if (m_KeyState[keyValue] == EKeyState::DOWN ||
			m_KeyState[keyValue] == EKeyState::HOLD)
			return m_KeyState[keyValue] = EKeyState::UP;
	}
	return m_KeyState[keyValue] = EKeyState::FREE;
}

EKeyState Input::getKeyState(const DWORD& keyValue)
{
	return m_KeyState[keyValue];
}

// �̺�Ʈ �ڵ鷯, �����쿡�� ���ڸ� �޾� ��� ó���� ����
void Input::MsgEvent(const MSG& message)
{
	// ��ũ�� ��ǥ ���, ��ũ��->Ŭ���̾�Ʈ ��ǥ ��ȯ
	GetCursorPos(&m_Cursor);
	ScreenToClient(Window::m_hWnd, &m_Cursor);
	_stprintf_s(m_Buffer, L"x : %d,  y : %d", m_Cursor.x, m_Cursor.y);

	switch (message.message)
	{
	case WM_LBUTTONDOWN:
	{
		m_MousePrevState[0] = EKeyState::FREE;
		m_MouseState[0] = EKeyState::DOWN;
	} break;
	case WM_LBUTTONUP:
	{
		m_MousePrevState[0] = EKeyState::HOLD;
		m_MouseState[0] = EKeyState::UP;
	} break;
	case WM_RBUTTONDOWN:
	{
		m_MousePrevState[1] = EKeyState::FREE;
		m_MouseState[1] = EKeyState::DOWN;
	} break;
	case WM_RBUTTONUP:
	{
		m_MousePrevState[1] = EKeyState::HOLD;
		m_MouseState[1] = EKeyState::UP;
	} break;
	case WM_MBUTTONDOWN:
	{
		m_MousePrevState[2] = EKeyState::FREE;
		m_MouseState[2] = EKeyState::DOWN;
	} break;
	case WM_MBUTTONUP:
	{
		m_MousePrevState[2] = EKeyState::HOLD;
		m_MouseState[2] = EKeyState::UP;
	} break;
	}
}

void Input::MouseUpdate()
{
	// ���콺 ���� ����
	for (int i = 0; i < 3; i++)
	{
		if (m_MousePrevState[i] == EKeyState::FREE)
		{
			if (m_MouseState[i] == EKeyState::DOWN)
			{
				m_MousePrevState[i] = EKeyState::DOWN;
				m_MouseState[i] = EKeyState::HOLD;
			}
		}
		else if (m_MousePrevState[i] == EKeyState::HOLD)
		{
			if (m_MouseState[i] == EKeyState::UP)
			{
				m_MousePrevState[i] = EKeyState::FREE;
				m_MouseState[i] = EKeyState::FREE;
			}
		}
	}
}

EKeyState Input::getMouseState(const int& mouseButton)
{
	return m_MouseState[mouseButton];
}

bool Input::Init()
{
	return true;
}

bool Input::Frame()
{
	if (KeyCheck(VK_ESCAPE) == EKeyState::DOWN)
	{
		isDebug = !isDebug;
	}

	KeyCheck(VK_F1);	KeyCheck(VK_F2);
	KeyCheck(VK_F3);	KeyCheck(VK_F4);
	KeyCheck(VK_LEFT);	KeyCheck(VK_RIGHT);
	KeyCheck(VK_UP);	KeyCheck(VK_DOWN);
	KeyCheck(VK_SPACE);	KeyCheck(VK_SHIFT);
	KeyCheck(VK_INSERT);KeyCheck(VK_DELETE);
	KeyCheck(VK_HOME);	KeyCheck(VK_END);
	KeyCheck(VK_ESCAPE);
	KeyCheck('Q');		KeyCheck('W');
	KeyCheck('E');		KeyCheck('R');
	KeyCheck('A');		KeyCheck('S');
	KeyCheck('D');		KeyCheck('F');
	KeyCheck('Z');		KeyCheck('X');
	KeyCheck('C');		KeyCheck('V');

	return true;
}

bool Input::Render()
{
	return true;
}

bool  Input::Release()
{
	return true;
}

POINT Input::getCursor()
{
	return m_Cursor;
}