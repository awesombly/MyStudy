#include "GameScene.h"




bool GameScene::Init() noexcept
{
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Enemy])
	{
		iter->setEnable(true);
	}
	m_ObjectManager.MapRect->setRect(*m_mapMap);
	m_ObjectManager.ReadMapScript();

	m_Player.setCenter(150, 250);
	m_Player.setDirection(EDirection::Right);
	m_SoundManager.setBGM("BGM/Mischievous Masquerade");

	// 버튼들
	//m_pIntroButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right * 0.31f, getClientRect().bottom * 0.88f, 150, 60 });
	//m_pIntroButton->setText(L"Back");
	//m_pIntroButton->setClickEvent(&MainClass::setIntroScene, this);

	m_pConfigButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right * 0.48f, getClientRect().bottom * 0.88f, 150, 60 });
	m_pConfigButton->setText(L"Config");
	m_pConfigButton->setClickEvent(&MainClass::ShowConfigure, this);

	m_pInvenButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right * 0.65f, getClientRect().bottom * 0.88f, 150, 60 });
	m_pInvenButton->setText(L"Inventory");
	m_pInvenButton->setClickEvent(&MainClass::ShowInventory, this);

	m_pEquipButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right* 0.82f, getClientRect().bottom * 0.88f, 150, 60 });
	m_pEquipButton->setText(L"Equip");
	m_pEquipButton->setClickEvent(&MainClass::ShowEquip, this);

	// 인터페이스(UI)
	m_ObjectManager.PushObject(m_MiniMap);
	m_ObjectManager.PushObject(m_ChatBox);
	m_ObjectManager.PushObject(&m_Inventory);
	//m_ObjectManager.PushObject(&m_Equipment);
	m_Inventory.setEnable(false);
	m_Equipment.setEnable(false);
	m_Settings.setEnable(false);
	m_HealthBar->setEnable(true);
	m_BloodRate->setEnable(true);
	m_MiniMap->setEnable(true);
	m_MiniMap->Render(m_DxManager.m_pD3dContext);
	m_MiniMap->setEnable(false);
	m_ChatBox->setEnable(false);

	for (int i = 0; i < 900; i++)
		m_ObjectManager.TakeObject(L"ScoreItem");
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Score])
		iter->setEnable(false, true);
	return true;
}

bool GameScene::Frame() noexcept
{
	// 모든 객체 행동
	m_DxManager.Frame();
	m_ObjectManager.Frame(m_Timer.SPF, m_Timer.AccumulateTime);

	// 리스폰
	static float responCount = 0.0f;
	responCount += Timer::SPF;
	if (responCount >= 5.0f)
	{
		responCount = 0.0f;
		m_ObjectManager.TakeObject(L"Fairy", { m_mapMap->getRight() + 100, rand() % m_mapMap->getBottom() });
	}
	// 엔딩
	if (m_Player.MyCenter.x > ObjectManager::MapRect->getRight() + 150)
		MainClass::setScene(ESceneName::Ending);
	return true;
}

bool GameScene::Render() noexcept
{
	// 점수표시
	static wostringstream scoreString;
	scoreString.str(L"");
	scoreString << L"Score : " << m_Player.Score;
	//WriteManager::Instance->SetFontSize(50);
	WriteManager::Instance->DrawTextW({ getClientRect().right * 0.88f , getClientRect().bottom * 0.16f, 300, 200 }, scoreString.str().c_str());
	// 드로우
	m_DxManager.Render();
	m_ObjectManager.Render(m_DxManager.m_pD3dContext);
	return true;
}

bool GameScene::Release() noexcept
{
	//m_pIntroButton->setEnable(false, true);
	//m_pConfigButton->setEnable(false, true);
	//m_pInvenButton->setEnable(false, true);
	//m_pEquipButton->setEnable(false, true);
	//m_Inventory.setEnable(false, true);
	//m_HealthBar->setEnable(false, true);
	//m_MiniMap->setEnable(false, true);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Enemy])
		iter->setEnable(false);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::UI])
		iter->setEnable(false);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Collider])
		iter->setEnable(false);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Image])
		iter->setEnable(false);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::EnemyAttack])
		iter->setEnable(false);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::PlayerAttack])
		iter->setEnable(false);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Effect])
		iter->setEnable(false);
	for (auto& iter : m_Inventory.m_QuickSlots)
		iter->setEnable(false);
	return true;
}