#pragma once
#include "MyActor.h"

class BackGround : public MyActor
{
protected:
	BackGround() = default;
public:
	BackGround(const wstring_view& myName, const EObjType& objType,
			 const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~BackGround() = default;

	virtual void UpdateVertexList(ID3D11DeviceContext* pDContext, const bool& useListUpdate = false) override;
	//virtual MyActor* clone();

	//virtual bool Init()		noexcept override;
	//virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()	noexcept override;*/
};

