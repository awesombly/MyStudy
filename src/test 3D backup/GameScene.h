#pragma once
#include "MainClass.h"

class GameScene : public MainClass
{
public:
	GameScene() = default;
	virtual ~GameScene() = default;
public:
	MyButton<MainClass>* m_pIntroButton;
	MyButton<MainClass>* m_pConfigButton;
	MyButton<MainClass>* m_pInvenButton;
	MyButton<MainClass>* m_pEquipButton;
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
};

