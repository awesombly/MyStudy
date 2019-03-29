#pragma once
#include "MainClass.h"

class IntroScene : public MainClass
{
public:
	IntroScene() = default;
	virtual ~IntroScene() = default;
public:
	MyActor* m_introBitmap;

	MyButton<MainClass>* m_pStartButton;
	MyButton<MainClass>* m_pConfigButton;
	MyButton<MainClass>* m_pEndButton;
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
};