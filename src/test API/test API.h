#pragma once
#include "Core.h"

class MainClass : public Core {
public:
	MainClass() = default;
	~MainClass() = default;
public:
	MyPlayer& m_player = MyPlayer::GetInstance();			// ���� ĳ����

	static MyBitMap m_backGround;							// �׳� ���
	static MyBitMap m_mapMap;								// �� �κ�

	static map<wstring, MainClass*> m_sceneList;
	static MainClass* m_curScene;
public:
	void setScene(const wstring& sceneName);
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};