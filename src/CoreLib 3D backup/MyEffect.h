#pragma once
#include "MyActor.h"


class MyEffect : public MyActor
{
public:
	using MyActor::MyActor;
	virtual ~MyEffect() = default;
public:
	virtual void EndSpriteEvent() override;
	virtual MyActor* clone() override;

	virtual bool Init() noexcept override;
	//virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)	noexcept override;

	virtual void UpdateTextureUV();
};