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
	const static float GravityPower;	// �߷� ũ��
	const static float BounceGravity;	// ƨ�� ���� ũ��
	MyRect m_prePosition;				// �浹 ���� ��ġ
	Point m_dirPoint;					// �̵� ���� ����

	int   m_healthPoint;				// ü��
	float m_rotation;					// ȸ�� ����
	float m_preRotation;				// ���� ȸ�� ����
	float m_curJumpPower;				// ���� ����(�߷�) ��
	float m_speed;						// �̵� �ӵ�
	Point m_bouncePower;				// ƨ��� ��
	Point m_bounceUnit;					// ƨ��� �ݴ� ���� ����
	Point  m_originWH;					// �⺻ ���� ũ��
	
	bool  m_isRepeat;					// ��������Ʈ �ݺ� ����
	bool  m_canHit;						// �ǰ� ���� ����
	bool  m_canNextSprite;
	bool  m_isDelete;
public:
	EDirection  m_curDirection;			// ���� ����

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