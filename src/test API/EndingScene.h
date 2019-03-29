#pragma once
#include "test API.h"

class EndingScene : public MainClass
{
public:
	EndingScene() = default;
	virtual ~EndingScene() = default;
public:
	MyBitMap m_endingBitmap;
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

