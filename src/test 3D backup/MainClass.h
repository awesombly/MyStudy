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
	MyPlayer& m_Player = MyPlayer::GetInstance();			// ���� ĳ����
	Inventory& m_Inventory = Inventory::GetInstance();		// �κ�
	Equipment& m_Equipment = Equipment::GetInstance();		// ���â
	Settings& m_Settings = Settings::GetInstance();			// ����â
	static HealthBar* m_HealthBar;							// ü�¹�
	static BloodRate* m_BloodRate;							// ����
	static MiniMap*	  m_MiniMap;							// �̴ϸ�
	static ChattingBox* m_ChatBox;							// ä�ùڽ�

	static MyActor* m_backGround;							// �׳� ���
	static MyActor* m_mapMap;								// �� �κ�
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