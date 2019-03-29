#include "EndingScene.h"
#include "PlayerBall.h"



bool EndingScene::Init() noexcept
{
	m_Player.setEnable(false);
	m_Player.setPoint(m_ObjectManager.MapRect->getRight() + 1000, 3000);
	m_SoundManager.setBGM("BGM/Ending");
	SoundManager::Instance->PlayVariation("shout", true, 2);

	// 공
	m_ObjectManager.SetProtoObject(new PlayerBall(L"PlayerBall", EObjType::UI));
	// 엔딩배경
	m_ObjectManager.PushObject(new MyActor(L"Lobby", EObjType::Image))
		->setRect(getClientRect().left + Window::DrawPoint.x, getClientRect().top + Window::DrawPoint.y,
				  getClientRect().right + Window::DrawPoint.x, getClientRect().bottom + Window::DrawPoint.y);
	// 클리어 글씨
	m_endingBitmap.resize(1);
	for (auto& iter : m_endingBitmap)
	{
		m_ObjectManager.PushObject(iter = new MyActor(L"gameClear", EObjType::UI))->setWH(1000, 300);
		iter->setCenter((getClientRect().right  * 0.5f) + Window::DrawPoint.x,
			(getClientRect().bottom * 0.5f) + Window::DrawPoint.y);
		m_ObjectManager.TakeObject(L"PlayerBall", { (getClientRect().right  * 0.5f) + Window::DrawPoint.x,
													(getClientRect().bottom * 0.5f) + Window::DrawPoint.y })
			->Init();
	}
	return true;
}

bool EndingScene::Frame() noexcept
{
	// 모든 객체 행동
	m_DxManager.Frame();
	m_ObjectManager.Frame(m_Timer.SPF, m_Timer.AccumulateTime);

	auto ending = m_endingBitmap.front();

	// 좌
	if (Input::Instance->getKeyState(VK_LEFT) == EKeyState::HOLD ||
		Input::Instance->getKeyState('A') == EKeyState::HOLD)
	{
		ending->Move(-200.0f * m_Timer.SPF, 0.0f);
	}
	else if (Input::Instance->getKeyState(VK_RIGHT) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('D') == EKeyState::HOLD)
	{
		ending->Move(200.0f * m_Timer.SPF, 0.0f);
	}
	// 우
	if (Input::Instance->getKeyState(VK_UP) == EKeyState::HOLD ||
		Input::Instance->getKeyState('W') == EKeyState::HOLD)
	{
		ending->Move(0.0f, -200.0f * m_Timer.SPF);
	}
	else if (Input::Instance->getKeyState(VK_DOWN) == EKeyState::HOLD ||
			 Input::Instance->getKeyState('S') == EKeyState::HOLD)
	{
		ending->Move(0.0f, 200.0f * m_Timer.SPF);
	}
	if (m_Input.getMouseState(0) == EKeyState::DOWN)
	{
		m_ObjectManager.TakeObject(L"PlayerBall", { m_Input.getCursor().x + Window::DrawPoint.x, m_Input.getCursor().y + Window::DrawPoint.y })
			->Init();
	}

	static int LR = 1;
	m_angle += m_Timer.SPF * 30;
	m_colorRate += m_Timer.SPF * LR;

	if (m_angle >= 6.28f)
	{
		m_angle -= 6.28f;
		m_ObjectManager.TakeObject(L"PlayerBall", { (getClientRect().right  * 0.5f) + Window::DrawPoint.x,
													(getClientRect().bottom * 0.5f) + Window::DrawPoint.y })
			->Init();
	}

	for (auto& iter : m_endingBitmap)
	{
		iter->Move(sinf(m_angle) * (rand() % 20), cosf(m_angle) * (rand() % 20));
		iter->SetColor((rand() % 10) * m_colorRate, (rand() % 10) * m_colorRate, (rand() % 10) * m_colorRate);
	}

	if (m_colorRate > 1.0f)
	{
		LR = -1;
		SoundManager::Instance->PlayVariation("shout", true, 2);
	}
	else if (m_colorRate < 0.05f)
	{
		LR = 1;
		SoundManager::Instance->PlayVariation("shout", true, 2);
	}
	
	return true;
}

bool EndingScene::Render() noexcept
{
	m_DxManager.Render();
	m_ObjectManager.Render(m_DxManager.m_pD3dContext);
	return true;
}

bool EndingScene::Release() noexcept
{
	m_DxManager.Release();
	m_ObjectManager.Release();
	m_SoundManager.Release();
	return true;
}