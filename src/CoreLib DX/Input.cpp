#include "Input.h"
#include "ChattingBox.h"
//nclude "../../include/server/MyProtocol.h"
#include "../../include/server/ClientServer.h"

bool Input::isDebug = false;
bool Input::isChatting = false;
POINT Input::m_Cursor = { 0, 0 };

Input::Input() : m_KeyState(), m_MouseState(), m_MousePrevState()
{
	Init();
}

Input::~Input() {}

EKeyState Input::KeyCheck(const DWORD& keyValue)
{
	// GetKeyState()		// 동기식
	// GetAsyncKeyState()	// 비동기식
	short sKey = GetAsyncKeyState(keyValue);
	// 클릭 상태일시 최상위 비트를 1로 바꿔줌
	if (sKey & 0x8000)		// 눌림			
	{
		if (m_KeyState[keyValue] == EKeyState::FREE)
			return m_KeyState[keyValue] = EKeyState::DOWN;
		else
			return m_KeyState[keyValue] = EKeyState::HOLD;
	}
	else                    // 안눌림
	{
		if (m_KeyState[keyValue] == EKeyState::DOWN ||
			m_KeyState[keyValue] == EKeyState::HOLD)
			return m_KeyState[keyValue] = EKeyState::UP;
	}
	return m_KeyState[keyValue] = EKeyState::FREE;
}

EKeyState Input::getKeyState(const DWORD& keyValue)
{
	if (isChatting)
		return EKeyState::FREE;
	return m_KeyState[keyValue];
}

// 이벤트 핸들러, 윈도우에서 인자를 받아 대신 처리를 수행
void Input::MsgEvent(const MSG& message)
{
	// 스크린 좌표 얻고, 스크린->클라이언트 좌표 전환
	GetCursorPos(&m_Cursor);
	ScreenToClient(Window::m_hWnd, &m_Cursor);
	//_stprintf_s(m_Buffer, L"x : %d,  y : %d", m_Cursor.x, m_Cursor.y);

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
	// 마우스 상태 갱신
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

bool Input::Init() noexcept
{
	return true;
}

bool Input::Frame() noexcept
{
	if (KeyCheck(VK_ESCAPE) == EKeyState::DOWN)
	{
		isDebug = !isDebug;
		isChatting = false;
	}
	// 엔터 체크
	if (KeyCheck(VK_RETURN) == EKeyState::DOWN && ChattingBox::Instance->isEnable())
	{
		if (isChatting && !ChattingBox::m_inputMsg.empty())
		{
			UPACKET* pPacket = ClientServer::Instance->TakePacket();
			pPacket->header.type = EPacket::CHAT_MSG;
			pPacket->header.length = (WORD)ChattingBox::m_inputMsg.size() + PACKET_HEADER_SIZE;
			memcpy(&pPacket->msg, Server::WCharToChar(ChattingBox::m_inputMsg.data()), pPacket->header.length);

			ClientServer::Instance->m_SendPacketPool.push(pPacket);
		}
		ChattingBox::m_inputMsg.clear();
		isChatting = !isChatting;
	}
	// 채팅 입력
	if (isChatting)
	{
		for (DWORD i = 0; i < 255; i++)
		{
			if (KeyCheck(i) == EKeyState::DOWN)
			{
				if (i == VK_SHIFT || i == VK_RETURN)
					continue;
				if (KeyCheck(VK_SHIFT) == EKeyState::FREE && 
					i >= 'A' && i <= 'Z')
				{
					ChattingBox::m_inputMsg +=
						[&](const DWORD& i, const char& val2, const char& val3) -> wchar_t {
							return (wchar_t)i + val2 - val3;
						}(i, 'a', 'A');
				}
				else
				{
					ChattingBox::m_inputMsg += (wchar_t)i;
				}
			}
		}
		return true;
	}

	KeyCheck(VK_F1);	KeyCheck(VK_F2);	KeyCheck(VK_F3);	KeyCheck(VK_F4);
	KeyCheck(VK_LEFT);	KeyCheck(VK_RIGHT);	KeyCheck(VK_UP);	KeyCheck(VK_DOWN);
	KeyCheck(VK_SPACE);	KeyCheck(VK_SHIFT);	KeyCheck(VK_ESCAPE);KeyCheck(VK_TAB);
	KeyCheck(VK_INSERT);KeyCheck(VK_DELETE);KeyCheck(VK_HOME);	KeyCheck(VK_END);
	KeyCheck(VK_NUMPAD0);	KeyCheck(VK_NUMPAD1);	KeyCheck(VK_NUMPAD2);	KeyCheck(VK_NUMPAD3);
	KeyCheck(VK_RETURN);	KeyCheck(VK_CONTROL);

	KeyCheck('1');		KeyCheck('2');		KeyCheck('3');		KeyCheck('4');
	KeyCheck('Q');		KeyCheck('W');		KeyCheck('E');		KeyCheck('R');
	KeyCheck('A');		KeyCheck('S');		KeyCheck('D');		KeyCheck('F');
	KeyCheck('Z');		KeyCheck('X');		KeyCheck('C');		KeyCheck('V');

	return true;
}

bool Input::Render() noexcept
{
	return true;
}

bool  Input::Release() noexcept
{
	return true;
}

POINT Input::getCursor()
{
	return m_Cursor;
}