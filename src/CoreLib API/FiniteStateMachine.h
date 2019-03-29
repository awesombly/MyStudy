#pragma once
#include "FiniteState.h"
#include "Singleton.h"


class FiniteStateMachine
{
private:
	// 특정 상태와 그 상태에 대한 이벤트 패턴을 저장하는 맵
	map<EState, FiniteState*> m_stateMap;
public:
	wstring m_Name;			// 해당 FSM(객체)의 이름
	//wstring m_Path;
	//int		m_index;
public:
	FiniteStateMachine(const wstring& myName);
	virtual ~FiniteStateMachine();
	
	// 새 상태-전이 추가
	bool	AddStateTransition(const EState& state, const EEvent& inputEvent, const EState& outputStateID);
	// 해당 상태의 전이 이벤트 삭제
	bool	DeleteTransition(const EState& state, const EEvent& inputEvent);
	// 현재 의미 없음
	EState	getCurrentState(const EState& state);
	// 해당 상태에 이벤트 발생시 전이 상태 반환
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
	// 모든 FSM 객체의 상태 전이 관계 등록
	bool Init();
	// 해당 이름의 FSM 객체가 없을시 생성하고 얻음
	FiniteStateMachine*	getFSM(const wstring& fsmName);
	//bool DeleteFSM(const int& index);
private:
	friend class Singleton<FSMManager>;
	FSMManager() = default;
public:
	~FSMManager();
};