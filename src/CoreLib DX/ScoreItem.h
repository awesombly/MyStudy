#pragma once
#include "MyObject.h"


class ScoreItem : public MyObject
{
protected:
	int		m_scoreValue;
protected:
	ScoreItem() = default;
public:
	ScoreItem(const wstring_view& myName, const EObjType& objType,
			 const string_view& vertexShaderName = "VShader", const string_view& pixelShaderName = "PShader");
	virtual ~ScoreItem() = default;

	virtual int GetScoreValue();
	virtual MyActor* clone() override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
};

