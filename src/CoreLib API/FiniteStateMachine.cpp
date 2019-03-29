#include "FiniteStateMachine.h"


// 모든 FSM 객체의 상태 전이 관계 등록
bool FSMManager::Init()
{
	FiniteStateMachine* pFSM = getFSM(L"Fairy");
	pFSM->AddStateTransition(EState::Basic, EEvent::FindTarget, EState::Attack);
	pFSM->AddStateTransition(EState::Basic, EEvent::NearPlayer, EState::Runaway);
	pFSM->AddStateTransition(EState::Basic, EEvent::Patrol, EState::Patrol);
	pFSM->AddStateTransition(EState::Basic, EEvent::Move, EState::Move);

	pFSM->AddStateTransition(EState::Attack, EEvent::LostTarget, EState::Move);
	pFSM->AddStateTransition(EState::Attack, EEvent::Move, EState::Move);
	pFSM->AddStateTransition(EState::Attack, EEvent::Patrol, EState::Patrol);
	pFSM->AddStateTransition(EState::Attack, EEvent::Stop, EState::Basic);

	pFSM->AddStateTransition(EState::Move, EEvent::FindTarget, EState::Attack);
	pFSM->AddStateTransition(EState::Move, EEvent::LostTarget, EState::Patrol);
	pFSM->AddStateTransition(EState::Move, EEvent::NearPlayer, EState::Runaway);
	pFSM->AddStateTransition(EState::Move, EEvent::Stop, EState::Basic);

	pFSM->AddStateTransition(EState::Patrol, EEvent::FindTarget, EState::Attack);
	pFSM->AddStateTransition(EState::Patrol, EEvent::LostTarget, EState::Move);
	pFSM->AddStateTransition(EState::Patrol, EEvent::Stop, EState::Basic);

	pFSM->AddStateTransition(EState::Runaway, EEvent::LostTarget, EState::Move);
	pFSM->AddStateTransition(EState::Runaway, EEvent::FindTarget, EState::Attack);
	pFSM->AddStateTransition(EState::Runaway, EEvent::NearPlayer, EState::Attack);
	pFSM->AddStateTransition(EState::Runaway, EEvent::Patrol, EState::Move);

	

	return true;
}

FiniteStateMachine*	FSMManager::getFSM(const wstring& fsmName)
{
	if(m_FSMMap.find(fsmName) == m_FSMMap.end())
		m_FSMMap[fsmName] = new FiniteStateMachine(fsmName);
	return m_FSMMap[fsmName];
}

//bool FSMManager::DeleteFSM(const int& index)
//{
//
//}

FSMManager::~FSMManager()
{
	for (auto& iter : m_FSMMap)
	{
		delete iter.second;
	}
	m_FSMMap.clear();
}





FiniteStateMachine::FiniteStateMachine(const wstring& myName) : m_Name(myName) { }

FiniteStateMachine::~FiniteStateMachine()
{
	for (auto& iter : m_stateMap)
		delete iter.second;
	m_stateMap.clear();
}

bool FiniteStateMachine::AddStateTransition(const EState& state, const EEvent& inputEvent, const EState& outputStateID)
{
	// 상태가 없을시 생성
	if (m_stateMap.find(state) == m_stateMap.end())
	{
		m_stateMap[state] = new FiniteState(state);
	}

	// state에 Event가 발생시 outState로 전환
	m_stateMap[state]->AddTransition(inputEvent, outputStateID);	
	return true;
}

bool FiniteStateMachine::DeleteTransition(const EState& state, const EEvent& inputEvent)
{
	auto iter = m_stateMap.find(state);
	if (iter == m_stateMap.end())
		return false;
	
	iter->second->DeleteTransition(inputEvent);
	return true;
}

// ??
EState FiniteStateMachine::getCurrentState(const EState& state)
{
	return m_stateMap[state]->getState();
}
// 상태 전이
EState FiniteStateMachine::getStateTransition(const EState& curState, const EEvent& event)
{
	if (m_stateMap.find(curState) == m_stateMap.end())
		return EState::Count;
	return m_stateMap[curState]->EventOutputState(event);
}