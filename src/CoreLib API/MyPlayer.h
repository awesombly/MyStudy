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

	map<EPlayerState, IPlayerState*> m_stateList;			// 상태별 행동
	IPlayerState*					 m_curState;			// 현재 상태
	IPlayerState*					 m_preState;			// 이전 상태
	wstring m_curSpriteName;

	bool m_isGround;		// 땅 체크
	bool m_isSword;			// 칼이냐

	float m_chargeCount;	// 충전 시간
	float m_delayFrame;		// 행동간 대기시간
	float m_delayAttack;	// 공격 간격
	float m_delayDodge;		// 회피 간격
	float m_delayCharge;	// 차지 쿨
	float m_delayHit;		// 피격 간격
public:
	static Point  MyCenter;
	
private:
	friend class Singleton<MyPlayer>;
	MyPlayer();
public:
	virtual ~MyPlayer();

	static void SynchPosition();							// 스크롤 위치 동기화

	virtual bool setState(const EPlayerState state);				// 상태 강제 전이
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