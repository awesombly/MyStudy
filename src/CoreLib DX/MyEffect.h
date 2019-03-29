#pragma once
#include "MyActor.h"


class MyEffect : public MyActor
{
public:
	MyEffect(const wstring_view& myName, const EObjType& objType,
			const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~MyEffect() = default;

	MyEffect() = default;
public:
	virtual void EndSpriteEvent() override;
	virtual MyActor* clone()	override;

	virtual bool Init() noexcept;
	//virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;

	virtual void UpdateTextureUV();
};