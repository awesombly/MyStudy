#include "MainScene.h"




bool GameScene::Init()
{
	MyBitMap::MapRect->setRect(m_mapMap);
	m_objectManager.ReadMapData(m_mapMap);

	m_player.setCenter(150, 250);
	m_player.m_curDirection = EDirection::Right;
	m_soundManager.setBGM("BGM/Mischievous Masquerade");
	return true;
}

bool GameScene::Frame()
{
	// ��� ��ü �ൿ
	m_objectManager.Frame();

	// ������
	static float responCount = 0.0f;
	responCount += Timer::SPF;
	if (responCount >= 7.0f)
	{
		responCount = 0.0f;
		m_objectManager.TakeObject(L"Fairy")->setCenter(m_mapMap.getRight() + 100, rand() % m_mapMap.getBottom());
	}

	if (m_player.MyCenter.x > MyBitMap::MapRect->getRight() + 150)
		MainClass::setScene(L"Ending");
	return true;
}

bool GameScene::Render()
{
	// ��� ��Ʈ�� ��ο�
	m_bitMapManager.Render();
	return true;
}

bool GameScene::Release()
{
	//m_bitMapManager.Release();
	//m_objectManager.Release();
	//m_soundManager.Release();
	return true;
}