#pragma once
#include "header.h"
//#include "fmod.h"
#include "fmod.hpp"			// fmod 헤더파일
#include "fmod_errors.h"	// "

#ifdef _WIN64 
#pragma comment( lib, "fmod64_vc.lib")
#else
#pragma comment( lib, "fmod_vc.lib")
#endif

class MySound
{
public:
	MySound();
	virtual ~MySound();
public:
	FMOD::Sound*	m_Sound;
	FMOD::Channel*	m_Channel;
	string			m_SoundName;

	unsigned int	m_TotalLength;
	unsigned int	m_CurrentLength;
	float			m_CurrentVolume;
public:
	unsigned int getTotalSecond();
	unsigned int getCurrentSecond();
	float getVolume();
	void  operVolume(const float& value);
	void  setVolume(const float& value);
};

