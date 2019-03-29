#pragma once
#include "MyBitMap.h"

enum class EDirection : bool {
	Left = 0,
	Right
};


class MyObject : public MyBitMap
{
public:
	MyObject();
	MyObject(const wstring& objName, const EObjType& type, const int& damage = 0);;
	virtual ~MyObject() = default;
protected:
	const static float GravityPower;	// 중력 크기
	const static float BounceGravity;	// 튕김 억제 크기
	MyRect m_prePosition;				// 충돌 이전 위치
	Point m_dirPoint;					// 이동 단위 벡터

	int   m_healthPoint;				// 체력
	float m_rotation;					// 회전 방향
	float m_preRotation;				// 이전 회전 방향
	float m_curJumpPower;				// 현재 점프(중력) 힘
	float m_speed;						// 이동 속도
	Point m_bouncePower;				// 튕기는 힘
	Point m_bounceUnit;					// 튕기는 반대 단위 벡터
	Point  m_originWH;					// 기본 상태 크기
	
	bool  m_isRepeat;					// 스프라이트 반복 여부
	bool  m_canHit;						// 피격 가능 여부
	bool  m_canNextSprite;
	bool  m_isDelete;
public:
	EDirection  m_curDirection;			// 현재 방향

	virtual void setTarget(const Point& target) override;
	virtual void setParameter(const float& speed, const int& damage, const float& rotation = 0.0f, const int& health = 100);
	virtual void setHp(const int& value);
	virtual void operHp(const int& value);
	virtual void setBounce(const float& angle, const int power);
	virtual void operBounce();
	virtual void operRotation(const float& angle);
	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true) { stateName; isRepeat; return false; };

	virtual void Move(const float& x, const float& y)	noexcept override;
	virtual void Move(const int& x, const int& y)		noexcept override;
	virtual void Move(const Point& point)				noexcept override;
	virtual void Move(const POINT& point)				noexcept override;
	virtual void setEnable(const bool& isEnable)				 override;
	virtual bool isEnable()										 override;
	virtual MyBitMap* clone()					  				 override;

	virtual int getHealth()
	{
		return m_healthPoint;
	}
public:
	virtual bool Init()		override;
	virtual bool Frame()	override;
	virtual bool Render()	override;
	virtual bool Release()	override;
};