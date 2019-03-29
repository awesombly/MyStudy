#pragma once
#include "MyObject.h"


class ScoreItem : public MyObject
{
protected:
	int		m_scoreValue;
public:
	using MyObject::MyObject;
	virtual ~ScoreItem() = default;

	virtual int GetScoreValue();
	virtual MyActor* clone() override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
};

