#pragma once
#include "FiniteState.h"
#include "Singleton.h"


class FiniteStateMachine
{
private:
	// Ư�� ���¿� �� ���¿� ���� �̺�Ʈ ������ �����ϴ� ��
	map<EState, FiniteState*> m_stateMap;
public:
	wstring m_Name;			// �ش� FSM(��ü)�� �̸�
	//wstring m_Path;
	//int		m_index;
public:
	FiniteStateMachine(const wstring& myName);
	virtual ~FiniteStateMachine();
	
	// �� ����-���� �߰�
	bool	AddStateTransition(const EState& state, const EEvent& inputEvent, const EState& outputStateID);
	// �ش� ������ ���� �̺�Ʈ ����
	bool	DeleteTransition(const EState& state, const EEvent& inputEvent);
	// ���� �ǹ� ����
	EState	getCurrentState(const EState& state);
	// �ش� ���¿� �̺�Ʈ �߻��� ���� ���� ��ȯ
	EState	getStateTransition(const EState& curState, const EEvent& event);
	//bool	Load(const wstring fileName);
};


class FSMManager : public Singleton<FSMManager>
{
public:
	map<wstring, FiniteStateMachine*>			m_FSMMap;
	map<wstring, FiniteStateMachine*>::iterator m_FSMIter;
	int	m_curIndex;
public:
	// ��� FSM ��ü�� ���� ���� ���� ���
	bool Init();
	// �ش� �̸��� FSM ��ü�� ������ �����ϰ� ����
	FiniteStateMachine*	getFSM(const wstring& fsmName);
	//bool DeleteFSM(const int& index);
private:
	friend class Singleton<FSMManager>;
	FSMManager() = default;
public:
	~FSMManager();
};