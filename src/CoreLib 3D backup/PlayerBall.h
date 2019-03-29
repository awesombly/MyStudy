#pragma once
#include "MyObject.h"

class PlayerBall : public MyObject
{
protected:
	float m_curAngle;
public:
	using MyObject::MyObject;
	virtual ~PlayerBall() = default;

	virtual void DeadEvent(MyActor* target) override { target; };
	virtual bool operBounce(const float& spf);
	virtual MyActor* clone() override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
};

