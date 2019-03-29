#pragma once
#include "MainClass.h"

class EndingScene : public MainClass
{
public:
	EndingScene() = default;
	virtual ~EndingScene() = default;
public:
	list<MyActor*> m_endingBitmap;
	float m_angle = 0.0f;
	float m_colorRate = 0.0f;
public:
	virtual bool Init()		noexcept override;
	virtual bool Frame()	noexcept override;
	virtual bool Render()	noexcept override;
	virtual bool Release()	noexcept override;
};

