#pragma once
#include "IEnemyState.h"
#include "MyPlayer.h"



class MyEnemy : public MyObject
{
protected:
	map<EState, IEnemyState*> m_stateList;			// 상태별 행동
	IEnemyState*			  m_curState;			// 현재 상태

	int   m_attackRadius;	// 적 공격 범위
	int   m_nearRadius;		// 접근 제어 범위

	float m_angle;			// 이동 방향
	float m_distance;		// 대 플레이어 거리
	float m_delayFrame;		// 행동간 대기시간
	float m_delayAttack;	// 공격 간격
	float m_delayDodge;		// 회피 간격
	float m_delayHit;		// 피격 간격
	
	bool m_canHit;
public:
	using MyObject::MyObject;
	virtual ~MyEnemy() = default;

	virtual void DeadEvent(MyActor* target) override;
	virtual bool Collision();
	virtual bool setState(const EState state);				// 상태 전이
	virtual EState getCurrentState();						// 현재 상태 반환

	virtual bool setSpriteState(const wstring& stateName, const bool& isRepeat = true);
	virtual bool operBounce(const float& spf) override;

	virtual bool Init() noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	//virtual bool Release()										noexcept override;
	virtual MyActor* clone();

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