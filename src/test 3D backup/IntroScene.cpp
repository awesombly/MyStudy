#include "IntroScene.h"



bool IntroScene::Init() noexcept
{
	// 사운드 로드
	m_SoundManager.Load("BGM/Mischievous Masquerade", false, FMOD_LOOP_NORMAL);
	m_SoundManager.Load("BGM/Sandbag Mini Game", false, FMOD_LOOP_NORMAL);
	m_SoundManager.Load("BGM/Brawl Breaks", false, FMOD_LOOP_NORMAL);
	m_SoundManager.Load("BGM/PLAY ROUGH", false, FMOD_LOOP_NORMAL);
	m_SoundManager.Load("BGM/Ending", false, FMOD_LOOP_NORMAL);
	m_SoundManager.setBGM("BGM/PLAY ROUGH");

	m_SoundManager.Load("bash1", false);
	m_SoundManager.Load("bash2", false);
	m_SoundManager.Load("bash3", false);
	m_SoundManager.Load("bashCharge1", false);
	m_SoundManager.Load("hit1", false);
	m_SoundManager.Load("hit2", false);
	m_SoundManager.Load("fairyShot1", false);
	m_SoundManager.Load("fairyShot2", false);
	m_SoundManager.Load("fairyShot3", false);
	m_SoundManager.Load("fairyShot4", false);
	m_SoundManager.Load("shout1", false);
	m_SoundManager.Load("shout2", false);
	m_SoundManager.Load("dead", false);
	m_SoundManager.Load("change", false);
	m_SoundManager.Load("dropItem", false);
	m_SoundManager.Load("explosion", false);
	m_SoundManager.Load("dash", false);
	m_SoundManager.Load("drink1", false);
	m_SoundManager.Load("drink2", false);
	m_SoundManager.Load("drink3", false);
	m_SoundManager.Load("equip1", false);
	m_SoundManager.Load("equip2", false);
	m_SoundManager.Load("buttonClick", false);
	m_SoundManager.Load("buttonRelease", false);

	//// 객체 등록
	MyActor* pActor = nullptr;

	m_ObjectManager.SetProtoObject((MyActor*)new MyEnemy(L"Fairy", EObjType::Enemy));

	((MyObject*)m_ObjectManager.SetProtoObject((MyActor*)new MyObject(L"FairyClash", EObjType::EnemyAttack)))
		->setParameter(100, 10, 200.0f);
	((MyObject*)m_ObjectManager.SetProtoObject((MyActor*)new MyObject(L"PlayerBomb", EObjType::PlayerAttack)))
		->setParameter(100, 50, 100.0f);
	((MyObject*)m_ObjectManager.SetProtoObject((MyActor*)new MyObject(L"FlamePotion", EObjType::Effect)))
		->SetColor(5.0, 0.8f, 0.8f);
	m_ObjectManager.SetProtoObject((MyActor*)new MyEffect(L"EffectSlash", EObjType::PlayerAttack))
		->m_damage = 50;
	m_ObjectManager.SetProtoObject((MyActor*)new MyEffect(L"EffectChargeSlash", EObjType::PlayerAttack))
		->m_damage = 100;

	pActor = (MyActor*)new MyEffect(L"FlameBomb", EObjType::PlayerAttack);
	pActor->SetColor(5.0f, 0.8f, 0.8f);
	m_ObjectManager.SetProtoObject(pActor)->m_damage = 100;

	m_ObjectManager.SetProtoObject((MyActor*)new MyEffect(L"Explosion", EObjType::Effect));
	m_ObjectManager.SetProtoObject((MyActor*)new MyEffect(L"Bomb", EObjType::Effect));
	m_ObjectManager.SetProtoObject((MyActor*)new MyEffect(L"Clash", EObjType::Effect));
	m_ObjectManager.SetProtoObject((MyActor*)new MyEffect(L"DashWind", EObjType::Effect));
	m_ObjectManager.SetProtoObject(new MyActor(L"None", EObjType::Wall));

	m_ObjectManager.SetProtoObject((MyActor*)new MyButton<MainClass>(L"Button", EObjType::UI));
	m_ObjectManager.SetProtoObject((MyActor*)new ScrollBar(L"ScrollBar", EObjType::UI));
	m_ObjectManager.SetProtoObject((MyActor*)new DropItem(L"ItemIcon", EObjType::Item))
		->setWH(30, 30);
	m_ObjectManager.SetProtoObject((MyActor*)new ScoreItem(L"ScoreItem", EObjType::Score, "VShader", "PRedColor"))
		->setWH(9, 9);

	// 플레이어
	m_Player.Init(L"Player", EObjType::Player);
	// 맵, 배경
	m_ObjectManager.PushObject(m_backGround = ((MyActor*)new BackGround(L"BackGround", EObjType::Image)))
		->setRect(0, 0, Window::getClientRect().right * 2, getClientRect().bottom);
	m_ObjectManager.PushObject(m_mapMap = (new MyActor(L"Map", EObjType::Image)))
		->setRect(0, 0, m_mapMap->m_width, Window::getClientRect().bottom + 130);

	// 버튼
	m_pStartButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right * 0.5f - 75, getClientRect().bottom * 0.35f, 150, 60 });
	m_pStartButton->setText(L"Start");
	m_pStartButton->setClickEvent(&MainClass::setMainScene, this);

	m_pConfigButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right * 0.5f - 75, getClientRect().bottom * 0.5f, 150, 60 });
	m_pConfigButton->setText(L"Config");
	m_pConfigButton->setClickEvent(&MainClass::ShowConfigure, this);

	m_pEndButton = (MyButton<MainClass>*)m_ObjectManager.TakeObject(L"Button", { getClientRect().right * 0.5f - 75, getClientRect().bottom * 0.65f, 150, 60 });
	m_pEndButton->setText(L"End");
	m_pEndButton->setClickEvent(&MainClass::ShowEnd, this);

	// 체력바
	m_ObjectManager.PushObject(m_HealthBar = new HealthBar(L"HpBar", EObjType::UI));
	m_HealthBar->setRect(getClientRect().right * 0.67f, getClientRect().bottom * 0.05f, getClientRect().right * 0.3f, getClientRect().bottom * 0.07f);
	m_HealthBar->Init();
	m_HealthBar->setEnable(false);
	// 혈압
	m_ObjectManager.PushObject(m_BloodRate = new BloodRate(L"BloodRate", EObjType::UI));
	m_BloodRate->setRect(getClientRect().right * 0.675f, getClientRect().bottom * 0.13f, getClientRect().right * 0.29f, getClientRect().bottom * 0.035f);
	m_BloodRate->Init();
	m_BloodRate->setEnable(false);
	// 미니맵
	m_MiniMap = new MiniMap(L"None", EObjType::UI, "VShader", "PPureColor");
	m_MiniMap->setRect(getClientRect().right * 0.05f, getClientRect().bottom * 0.03f, getClientRect().right * 0.6f, getClientRect().bottom * 0.15f);
	m_MiniMap->Init(); 
	m_MiniMap->setEnable(false);
	// 채팅창
	m_ChatBox = new ChattingBox(L"None", EObjType::UI, "VShader", "PPureColor");
	m_ChatBox->setRect(getClientRect().right * 0.55f, getClientRect().bottom * 0.5f, getClientRect().right * 0.4f, getClientRect().bottom * 0.35f);
	m_ChatBox->Init();
	m_ChatBox->setEnable(false);
	// 인벤토리
	m_Inventory.setRect(getClientRect().right * 0.55f, getClientRect().bottom * 0.1f, getClientRect().right * 0.4f, getClientRect().bottom * 0.85f);
	m_Inventory.Init(L"Inventory", EObjType::UI);
	m_Inventory.setEnable(false);
	// 장비창
	m_Equipment.setRect(getClientRect().right * 0.05f, getClientRect().bottom * 0.1f, getClientRect().right * 0.4f, getClientRect().bottom * 0.7f);
	m_Equipment.Init(L"Equipment", EObjType::UI);
	m_Equipment.setEnable(false);
	// 환경설정
	m_Settings.setRect(getClientRect().right * 0.2f, getClientRect().bottom * 0.1f, getClientRect().right * 0.6f, getClientRect().bottom * 0.85f);
	m_Settings.Init(L"Settings", EObjType::UI);
	m_Settings.setEnable(false);

	// 화면 이동 고정
	m_ObjectManager.MapRect->setRect(getClientRect());

	// 인트로 화면, 발판
	m_ObjectManager.PushObject(m_introBitmap = new MyActor(L"Intro", EObjType::Image))
								->setRect(getClientRect());
	m_ObjectManager.PushObject(new MyActor(L"ObjectNeedle", EObjType::Collider))
								->setRect(getClientRect().right * 0.15f, getClientRect().bottom * 0.6f, getClientRect().right * 0.25f, getClientRect().bottom * 0.1f);
	m_ObjectManager.PushObject(new MyActor(L"ObjectGreen", EObjType::Collider))
								->setRect(getClientRect().right * 0.6f, getClientRect().bottom * 0.4f, getClientRect().right * 0.4f, getClientRect().bottom * 0.1f);
	m_ObjectManager.TakeObject(L"None", { 0, (int)(getClientRect().bottom * 0.76f), getClientRect().right , 999999999 });

	// 적
	for (int i = 0; i < 30; i++)
	{
		m_ObjectManager.TakeObject(L"Fairy", { rand() % (m_mapMap->getRight() - 2000) + 2000, rand() % m_mapMap->getBottom() })
					->setEnable(false);
	}

	return true;
}

bool IntroScene::Frame() noexcept
{
	m_DxManager.Frame();
	m_ObjectManager.Frame(m_Timer.SPF, m_Timer.AccumulateTime);
	return true;
}
 
bool IntroScene::Render() noexcept
{
	m_DxManager.Render();
	m_ObjectManager.Render(m_DxManager.GetDContext());
	static float loadingTime = 0.0f;
	loadingTime += m_Timer.SPF;
	if (loadingTime < 5.0f)
	{
		float colorRate = (5.5f - loadingTime) * 0.2f;
		m_loadingBitmap->SetColor(colorRate, colorRate, colorRate);
		m_loadingBitmap->Render(m_DxManager.GetDContext());
	}
	else
	{
		if (!(m_loadingBitmap == nullptr))
		{
			m_ObjectManager.DeleteObject(m_loadingBitmap);
			m_Player.setCenter(getClientRect().right * 0.27f, -1350.0f);
			m_loadingBitmap = nullptr;
		}
	}
	return true;
}

bool IntroScene::Release() noexcept
{
	m_pStartButton->setEnable(false, true);
	m_pConfigButton->setEnable(false, true);
	m_pEndButton->setEnable(false, true);
	m_ObjectManager.DeleteObject(m_introBitmap);
	//m_introBitmap->setEnable(false, true);
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Wall])
		delete (MyRect*)iter;
	for (auto& iter : m_ObjectManager.m_ObjectList[EObjType::Collider])
		delete iter;
	m_ObjectManager.m_ObjectList[EObjType::Wall].clear();
	m_ObjectManager.m_ObjectList[EObjType::Collider].clear();
	return true;
}