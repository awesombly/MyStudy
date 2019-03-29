#pragma once
#include "MyActor.h"


class MyObject : public MyActor
{
protected:
	const static float GravityPower;	// 중력 크기
	const static float BounceGravity;	// 튕김 억제 크기

	MyRect		m_prevPosition;			// 이전 프레임 위치
	MyVector2	m_dirPoint;				// 이동 단위 벡터

	float		m_healthPoint;			// 체력
	float		m_speed;				// 이동 속도
	float		m_curGravityPower;		// 현재 중력 힘
	MyVector2	m_bouncePower;			// 튕기는 힘
	MyVector2	m_bounceUnit;			// 튕기는 단위 벡터
	MyVector2	m_originWH;				// 기본 상태 크기

	bool		m_isRepeat;				// 스프라이트 반복 여부
	float		m_curSPF;				// spf 대입용
	float		m_delayDelete;			// 삭제시간

	bool		m_canNextSprite;
public:
	using MyActor::MyActor;
	virtual ~MyObject() = default;

	virtual void EndSpriteEvent() override;
	virtual void DeadEvent(MyActor* target) override;
	virtual bool CollisionWall(MyRect* object, const bool& bottomCheck = true);
	virtual void setParameter(const int& health, const int& damage, const float& speed);
	virtual void setTarget(const MyVector2& targetPos);
	virtual void setGravityPower(const float& gravity);
	virtual void setBounce(const float& angle, const int power);
	virtual bool operBounce(const float& spf);
	virtual void setHp(const float& value);
	virtual void operHp(const float& value);
	virtual int	 getHealth();

	virtual void UpdateTextureUV() override;
	virtual void Move(const int& x, const int& y)		noexcept override;
	virtual void Move(const float& x, const float& y)	noexcept override;
	virtual void Move(const MyVector2& point)			noexcept override;
	virtual void Move(const POINT& point)				noexcept override;
	virtual bool isEnable() override;
	virtual MyActor* clone() override;

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	//virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
};