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

// ���� ����->�̺�Ʈ �߻�->���� ����
class FiniteState
{
private:
	friend class FiniteStateMachine;
public:
	EState			    m_myState;		
	map<EEvent, EState> m_stateTransMap;	// �������̸�
public:
	// �̺�Ʈ �߰�(�Է�-���)
	void	AddTransition(const EEvent& inputEvent, const EState& outputStateID);
	void	DeleteTransition(const EEvent& inputEvent);
	EState	EventOutputState(const EEvent& inputEvent);
	
	EState	getState();				// ���� ��ȯ
	int		getCount();				// ��ü Ʈ������ �� ��ȯ
	
	FiniteState(const EState& state);
	virtual ~FiniteState();
};