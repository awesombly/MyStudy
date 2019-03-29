#pragma once
#include "test API.h"

class IntroScene : public MainClass
{
public:
	IntroScene() = default;
	virtual ~IntroScene() = default;
public:
	MyBitMap* m_introBitmap;
	MyBitMap* m_introObject;
	MyBitMap* m_introObject2;
	MyRect*   m_introRect;
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};