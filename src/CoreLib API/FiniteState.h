#pragma once
#include "header.h"

enum class EState : char
{
	Basic = 0,
	Move,
	Patrol,
	Attack,
	Runaway,
	Hit,
	Dead,
	Count
};

enum class EEvent : char 
{
	FindTarget = 0,
	LostTarget,
	Hit,
	NearPlayer,
	Stop,
	Move,
	Patrol,
};

class FiniteStateMachine;

// 이전 상태->이벤트 발생->전이 상태
class FiniteState
{
private:
	friend class FiniteStateMachine;
public:
	EState			    m_myState;		
	map<EEvent, EState> m_stateTransMap;	// 상태전이맵
public:
	// 이벤트 추가(입력-출력)
	void	AddTransition(const EEvent& inputEvent, const EState& outputStateID);
	void	DeleteTransition(const EEvent& inputEvent);
	EState	EventOutputState(const EEvent& inputEvent);
	
	EState	getState();				// 상태 반환
	int		getCount();				// 전체 트랜지션 수 반환
	
	FiniteState(const EState& state);
	virtual ~FiniteState();
};