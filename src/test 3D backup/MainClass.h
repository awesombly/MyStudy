#pragma once
#include "CoreDX.h"
#include "MyEnemy.h"
#include "MiniMap.h"
#include "Settings.h"
#include "BackGround.h"
#include "MyEffect.h"
#include "ChattingBox.h"

enum class ESceneName : char {
	Intro = 0,
	Main,
	Ending,
};

class MainClass : public Core 
{
public:
	MainClass() = default;
	~MainClass() = default;
public:
	MyPlayer& m_Player = MyPlayer::GetInstance();			// 메인 캐릭터
	Inventory& m_Inventory = Inventory::GetInstance();		// 인벤
	Equipment& m_Equipment = Equipment::GetInstance();		// 장비창
	Settings& m_Settings = Settings::GetInstance();			// 설정창
	static HealthBar* m_HealthBar;							// 체력바
	static BloodRate* m_BloodRate;							// 혈압
	static MiniMap*	  m_MiniMap;							// 미니맵
	static ChattingBox* m_ChatBox;							// 채팅박스

	static MyActor* m_backGround;							// 그냥 배경
	static MyActor* m_mapMap;								// 맵 부분
	static MyActor* m_loadingBitmap;

	static map<ESceneName, MainClass*> m_SceneList;
	static MainClass* m_curScene;
public:
	void setScene(const ESceneName& sceneName);
	void setIntroScene();
	void setMainScene();
	void ShowEnd();
	void ShowConfigure();
	void ShowInventory();
	void ShowEquip();
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
};