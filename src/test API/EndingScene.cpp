#include "EndingScene.h"




bool EndingScene::Init()
{
	//m_player.setEnable(false);
	m_player.setPoint(MyBitMap::MapRect->getRight() + 1000, 3000);
	m_soundManager.setBGM("BGM/Ending");
	m_bitMapManager.AddObjBitMap(L"gameClear", &m_endingBitmap)->setWH(1300, 300);
	return true;
}

bool EndingScene::Frame()
{
	// 모든 객체 행동
	m_objectManager.Frame();
	static int LR = -1;
	m_soundManager.Frame();
	m_player.Move(0.8f * LR, 0.0f);
	m_endingBitmap.setCenter((int)m_player.getCenter().x, 300);
	
	if (m_player.MyCenter.x < getClientRect().right / 2)
	{
		LR = 1;
		SoundManager::Instance->PlayVariation("shout", true, 2);
	}
	else if (m_player.MyCenter.x > MyBitMap::MapRect->getRight() - getClientRect().right / 2)
	{
		LR = -1;
		SoundManager::Instance->PlayVariation("shout", true, 2);
	}
	return true;
}

bool EndingScene::Render()
{
	// 모든 비트맵 드로우
	m_bitMapManager.Render();
	return true;
}

bool EndingScene::Release()
{
	m_bitMapManager.Release();
	m_objectManager.Release();
	m_soundManager.Release();
	return true;
}