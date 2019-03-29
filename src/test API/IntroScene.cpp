#include "IntroScene.h"




bool IntroScene::Init()
{
	if (!FSMManager::GetInstance().Init() || !m_bitMapManager.Init() || !m_objectManager.Init() || !m_soundManager.Init())
		return false;

	m_introBitmap = new MyBitMap(L"Intro");
	m_introBitmap->setRect(getClientRect());
	
	//m_bitMapManager.AddObjBitMap(L"Intro", m_introBitmap)->setRect(getClientRect());
	m_soundManager.Load("BGM/Mischievous Masquerade", false, FMOD_LOOP_NORMAL);
	m_soundManager.Load("BGM/Sandbag Mini Game", false, FMOD_LOOP_NORMAL);
	m_soundManager.Load("BGM/Brawl Breaks", false, FMOD_LOOP_NORMAL);
	m_soundManager.Load("BGM/PLAY ROUGH", false, FMOD_LOOP_NORMAL);
	m_soundManager.Load("BGM/Ending", false, FMOD_LOOP_NORMAL);
	m_soundManager.setBGM("BGM/PLAY ROUGH");

	m_soundManager.Load("bash1", false);
	m_soundManager.Load("bash2", false);
	m_soundManager.Load("bash3", false);
	m_soundManager.Load("bashCharge1", false);
	m_soundManager.Load("hit1", false);
	m_soundManager.Load("hit2", false);
	m_soundManager.Load("fairyShot1", false);
	m_soundManager.Load("fairyShot2", false);
	m_soundManager.Load("fairyShot3", false);
	m_soundManager.Load("fairyShot4", false);
	m_soundManager.Load("shout1", false);
	m_soundManager.Load("shout2", false);
	m_soundManager.Load("dead", false);
	m_soundManager.Load("change", false);
	m_soundManager.Load("explosion", false);
	
	// 배경
	m_bitMapManager.AddObjBitMap(L"BackGround", &m_backGround);
	m_backGround.setRect(-300, 0, 
						 (int)(m_backGround.getWidth() * 2.0f), 
						 (int)(m_backGround.getHeight() * 2.0f));
	
	// 맵 지형
	m_bitMapManager.AddObjBitMap(L"Map", &m_mapMap)
		->setRect(0, 30, m_mapMap.m_width, Window::getClientRect().bottom -30 );

	// 적
	for (int i = 0; i < 30; i++)
	{
		m_objectManager.TakeObject(L"Fairy")
			->setCenter(rand() % m_mapMap.getWidth() + 2000, rand() % m_mapMap.getBottom());
	}   
	MyBitMap::MapRect->setRect(getClientRect());
	Timer::SPF = 0.0f;

	// 플레이어, 인트로 화면
	m_introObject = new MyBitMap(L"ObjectNeedle");
	m_introObject->setRect(630, 350, 250, 40);
	m_introObject2 = new MyBitMap(L"ObjectGreen");
	m_introObject2->setRect(1100, 230, 500, 30);
	m_player.Init();
	m_player.setCenter(924, -5500);
	m_introRect = new MyRect();
	m_introRect->setRect(483, 432, 1300, 10000);
	m_objectManager.AddWall(m_introObject);
	m_objectManager.AddWall(m_introObject2);
	m_objectManager.AddWall(m_introRect);
	return true;
}

bool IntroScene::Frame()
{
	// 키 검사 -> 씬 전환
	if (m_Input.getKeyState(VK_SPACE) == EKeyState::UP)
	{
		MainClass::setScene(L"Main");
	}
	m_player.Frame();
	for (auto& iter : m_objectManager.m_ObjectList[EObjType::PlayerAttack])
	{
		iter->Frame();
	}
	return true;
}

bool IntroScene::Render()
{
	// 배경 뿌리기
	m_introBitmap->Render();
	m_introObject->Render();
	m_introObject2->Render();
	m_player.Render();
	for (auto& iter : m_bitMapManager.m_objBitMap)
	{
		if (iter->m_objType == EObjType::PlayerAttack)
			iter->Render();
	}
	return true;
}

bool IntroScene::Release()
{
	delete m_introRect;
	m_objectManager.ClearWall();
	delete m_introBitmap;
	m_bitMapManager.RemoveBitMap(m_introBitmap);
	delete m_introObject;
	m_bitMapManager.RemoveBitMap(m_introObject);
	delete m_introObject2;
	m_bitMapManager.RemoveBitMap(m_introObject2);
	return true;
}