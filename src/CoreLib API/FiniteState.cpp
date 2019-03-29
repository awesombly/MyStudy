#include "FiniteState.h"



FiniteState::FiniteState(const EState& state) : m_myState(state) {}

FiniteState::~FiniteState() 
{
	for (auto iter = m_stateTransMap.begin();
			  iter != m_stateTransMap.end();)
	{
		m_stateTransMap.erase(iter);
	}
}


void FiniteState::AddTransition(const EEvent& inputEvent, const EState& outputStateID)
{
	m_stateTransMap[inputEvent] = outputStateID;
}

void FiniteState::DeleteTransition(const EEvent& inputEvent)
{
	m_stateTransMap.erase(inputEvent);
}

EState FiniteState::EventOutputState(const EEvent& inputEvent)
{
	auto iter = m_stateTransMap.find(inputEvent);
	if (iter == m_stateTransMap.end())
		return EState::Count;
	return iter->second;
}

EState FiniteState::getState()
{
	return m_myState;
}

int	FiniteState::getCount()
{
	return (int)m_stateTransMap.size();
}