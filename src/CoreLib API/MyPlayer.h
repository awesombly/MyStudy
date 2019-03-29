#pragma once
#include "MyObject.h"
#include "Input.h"
#include "Singleton.h"
#include "IPlayerState.h"


class MyPlayer : public MyObject, public Singleton<MyPlayer>
{
protected:
	Point  m_preCursor;
	const float m_MaxGravity;
	const float m_JumpPower;

	map<EPlayerState, IPlayerState*> m_stateList;			// ���º� �ൿ
	IPlayerState*					 m_curState;			// ���� ����
	IPlayerState*					 m_preState;			// ���� ����
	wstring m_curSpriteName;

	bool m_isGround;		// �� üũ
	bool m_isSword;			// Į�̳�

	float m_chargeCount;	// ���� �ð�
	float m_delayFrame;		// �ൿ�� ���ð�
	float m_delayAttack;	// ���� ����
	float m_delayDodge;		// ȸ�� ����
	float m_delayCharge;	// ���� ��
	float m_delayHit;		// �ǰ� ����
public:
	static Point  MyCenter;
	
private:
	friend class Singleton<MyPlayer>;
	MyPlayer();
public:
	virtual ~MyPlayer();

	static void SynchPosition();							// ��ũ�� ��ġ ����ȭ

	virtual bool setState(const EPlayerState state);				// ���� ���� ����
	virtual bool HeadCheck();
	virtual bool Collision();
	virtual bool KeyInput();
	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true) override;


	static Point getCenter();
	virtual void setWH(const int& width, const int& height)		noexcept override;
	virtual void setWH(const float& width, const float& height)	noexcept override;
	virtual void setWH(const Point& width_height)				noexcept override;
	virtual void setBottom(const int& bottom)			noexcept;

	virtual bool Init()   override;
	virtual bool Frame()  override;
	virtual bool Render() override;
	virtual MyBitMap* clone() override;

	friend class IPlayerState;
	friend class PlayerStateBasic;
	friend class PlayerStateCrawling;
	friend class PlayerStateHit;
	friend class PlayerStateSword;
	friend class PlayerStateAttack;
	friend class PlayerStateSkill;
	friend class PlayerStateDead;
};