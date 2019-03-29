#pragma once
#include "test API.h"

class GameScene : public MainClass
{
public:
	GameScene() = default;
	virtual ~GameScene() = default;
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};

