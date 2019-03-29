#include "SoundManager.h"
#include "Input.h"
#include "Timer.h"



// 사운드 파일 로드
void SoundManager::Load(const string& soundName, const bool& isPlay, const int& option)
{
	m_SoundList[soundName].m_SoundName = soundName;
	string soundPath = "../../data/sound/" + soundName + ".mp3";
	m_pSystem->createSound(soundPath.c_str(), option, 0, &(m_SoundList[soundName].m_Sound));

	if (isPlay)
	{
		m_pSystem->playSound(m_SoundList[soundName].m_Sound, 0, false, &m_SoundList[soundName].m_Channel);
	}
}

void SoundManager::setBGM(const string& soundName)
{
	if( m_curBGM != nullptr)
		Stop(m_curBGM->m_SoundName);

	m_curBGM = &m_SoundList[soundName];
	Play(soundName, true);
}

// isPlay가 true시 재생중이 아닐때만 재생?
void SoundManager::Play(const string& soundName, const bool& isPlay)
{
	bool isPlaying = false;
	if (isPlay)
	{ 
		if (m_SoundList[soundName].m_Channel != NULL)
		{
			m_SoundList[soundName].m_Channel->isPlaying(&isPlaying);
		}
	}
	if (!isPlaying)
	{
		m_pSystem->playSound(m_SoundList[soundName].m_Sound, 0, false, &m_SoundList[soundName].m_Channel);
	}
}

void SoundManager::PlayVariation(string&& soundName, const bool& isPlay, const int& vCount)
{
	soundName = soundName + (char)('1' + (rand() % vCount));
	Play(soundName, isPlay);
}

void SoundManager::Stop(const string& soundName)
{
	m_SoundList[soundName].m_Channel->stop();
}

void SoundManager::Paused(const string& soundName)
{
	bool isPaused;
	m_SoundList[soundName].m_Channel->getPaused(&isPaused);
	m_SoundList[soundName].m_Channel->setPaused(!isPaused);
}

void SoundManager::Volume(const string& soundName, const float& value)
{
	m_SoundList[soundName].operVolume(value);
}


bool SoundManager::Init()
{
	FMOD::System_Create(&m_pSystem);
	//		       채널수, 속성
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_curBGM = nullptr;
	return true;
}

bool SoundManager::Frame()
{
	m_pSystem->update();
	
	// 음량 조절
	if (Input::Instance->getKeyState(VK_HOME) == EKeyState::HOLD)
	{
		if (m_curBGM != nullptr)
			Volume(m_curBGM->m_SoundName, 2.0f * Timer::SPF);
	}
	if (Input::Instance->getKeyState(VK_END) == EKeyState::HOLD)
	{
		if (m_curBGM != nullptr)
			Volume(m_curBGM->m_SoundName, -2.0f * Timer::SPF);
	}
	if (Input::Instance->getKeyState(VK_INSERT) == EKeyState::HOLD)
	{
		for (auto& iter : m_SoundList)
		{
			if (&iter.second != m_curBGM)
				Volume(iter.first, 2.0f * Timer::SPF);
		}
	}
	if (Input::Instance->getKeyState(VK_DELETE) == EKeyState::HOLD)
	{
		for (auto& iter : m_SoundList)
		{
			if (&iter.second != m_curBGM)
				Volume(iter.first, -2.0f * Timer::SPF);
		}
	}

	if (Input::isDebug)
	{
		/*
		_stprintf_s(m_Buffer, L"BGM [%02d : %02d]/[%02d : %02d]",
					m_SoundList["BGM"].getTotalSecond() / 60,
					m_SoundList["BGM"].getTotalSecond() % 60,
					m_SoundList["BGM"].getCurrentSecond() / 60,
					m_SoundList["BGM"].getCurrentSecond() % 60);*/
	}
	return true;
}

bool SoundManager::Render()
{
	return true;
}

bool SoundManager::Release()
{
	// 해제 처리
	for (auto& iter : m_SoundList)
	{
		iter.second.m_Sound->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}
