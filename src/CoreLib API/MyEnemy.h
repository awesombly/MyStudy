#pragma once
#include "MyObject.h"
#include "IEnemyState.h"
#include "MyPlayer.h"


// 행동에 확률 적용

class MyEnemy : public MyObject
{
public:
	friend class IEnemyState;
	MyEnemy(const wstring& objName);
	virtual ~MyEnemy() = default;
protected:
	//static map<EState, wstring> StateToString;	// 잉여
	map<EState, IEnemyState*> m_stateList;			// 상태별 행동
	IEnemyState*			  m_curState;			// 현재 상태
	FiniteStateMachine*		  m_myFSM;				// FSM 객체(안씀)
	
	int   m_attackRadius;	// 적 공격 범위
	int   m_nearRadius;		// 접근 제어 범위
	
	float m_angle;			// 이동 방향
	float m_distance;		// 대 플레이어 거리
	float m_delayFrame;		// 행동간 대기시간
	float m_delayAttack;	// 공격 간격
	float m_delayDodge;		// 회피 간격
	float m_delayHit;		// 피격 간격
public:
	virtual bool  setState(const EState state);				// 상태 강제 전이
	// FSM 함수인데 안쓰는게 나은듯
	virtual EState getCurrentState();						// 현재 상태 반환
	virtual bool  setEventTransition(const EEvent& event);	// 이벤트 발생 전이
	virtual void  setFSM();									// FSM 세팅
	
	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true) override;
	virtual void operBounce() override;

	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual MyBitMap* clone() override;

	friend class IEnemyState;
	friend class EnemyStateBasic;
	friend class EnemyStateAttack;
	friend class EnemyStateDead;
	friend class EnemyStateHit;
	friend class EnemyStateMove;
	friend class EnemyStatePatrol;
	friend class EnemyStatePatrol;
	friend class EnemyStateRunaway;
};