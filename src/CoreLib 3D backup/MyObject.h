#pragma once
#include "MyActor.h"


class MyObject : public MyActor
{
protected:
	const static float GravityPower;	// �߷� ũ��
	const static float BounceGravity;	// ƨ�� ���� ũ��

	MyRect		m_prevPosition;			// ���� ������ ��ġ
	MyVector2	m_dirPoint;				// �̵� ���� ����

	float		m_healthPoint;			// ü��
	float		m_speed;				// �̵� �ӵ�
	float		m_curGravityPower;		// ���� �߷� ��
	MyVector2	m_bouncePower;			// ƨ��� ��
	MyVector2	m_bounceUnit;			// ƨ��� ���� ����
	MyVector2	m_originWH;				// �⺻ ���� ũ��

	bool		m_isRepeat;				// ��������Ʈ �ݺ� ����
	float		m_curSPF;				// spf ���Կ�
	float		m_delayDelete;			// �����ð�

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