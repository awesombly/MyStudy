#include "MySound.h"



MySound::MySound()
{}


MySound::~MySound()
{}

unsigned int MySound::getTotalSecond()
{
	//m_Sound->getLength(&m_TotalLength, FMOD_TIMEUNIT_MS);
	return m_TotalLength / 1000;
}

unsigned int MySound::getCurrentSecond()
{
	m_Channel->getPosition(&m_CurrentLength, FMOD_TIMEUNIT_MS);
	return m_CurrentLength / 1000;
}

float MySound::getVolume()
{
	m_Channel->getVolume(&m_CurrentVolume);
	return m_CurrentVolume;
}

void  MySound::operVolume(const float& value)
{
	getVolume();
	m_CurrentVolume += value;
	if (m_CurrentVolume > 100.0f)
		m_CurrentVolume = 100.0f;
	if (m_CurrentVolume < 0.0f)
		m_CurrentVolume = 0.0f;

	m_Channel->setVolume(m_CurrentVolume);
}

void  MySound::setVolume(const float& value)
{
	m_CurrentVolume = value;
	if (m_CurrentVolume > 100.0f)
		m_CurrentVolume = 100.0f;
	if (m_CurrentVolume < 0.0f)
		m_CurrentVolume = 0.0f;

	if (m_Channel == nullptr)
		return;
	m_Channel->setVolume(m_CurrentVolume);
}