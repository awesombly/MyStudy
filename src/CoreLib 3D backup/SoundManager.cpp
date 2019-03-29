#include "SoundManager.h"
#include "Input.h"
#include "Timer.h"



// ���� ���� �ε�
void SoundManager::Load(const string_view& soundName, const bool& isPlay, const int& option)
{
	m_SoundList[soundName.data()].m_SoundName = soundName.data();
	string soundPath = "../../data/sound/"s + soundName.data() + ".mp3";
	m_pSystem->createSound(soundPath.c_str(), option, 0, &(m_SoundList[soundName.data()].m_Sound));

	if (isPlay)
	{
		m_pSystem->playSound(m_SoundList[soundName.data()].m_Sound, 0, false, &m_SoundList[soundName.data()].m_Channel);
	}
}

void SoundManager::setBGM(const string_view& soundName)
{
	if( m_curBGM != nullptr)
		Stop(m_curBGM->m_SoundName);

	m_curBGM = &m_SoundList[soundName.data()];
	Play(soundName.data(), true);
}

// isPlay�� true�� ������� �ƴҶ��� ���?
void SoundManager::Play(const string_view& soundName, const bool& isPlay)
{
	bool isPlaying = isPlay;
	if (false)
	{ 
		if (m_SoundList[soundName.data()].m_Channel != NULL)
		{
			m_SoundList[soundName.data()].m_Channel->isPlaying(&isPlaying);
		}
	}
	if (!isPlaying)
	{
		m_pSystem->playSound(m_SoundList[soundName.data()].m_Sound, 0, false, &m_SoundList[soundName.data()].m_Channel);
	}
}

void SoundManager::PlayVariation(string&& soundName, const bool& isPlay, const int& vCount)
{
	soundName = soundName + (char)('1' + (rand() % vCount));
	Play(soundName, isPlay);
}

void SoundManager::Stop(const string_view& soundName)
{
	m_SoundList[soundName.data()].m_Channel->stop();
}

void SoundManager::Paused(const string_view& soundName)
{
	bool isPaused;
	m_SoundList[soundName.data()].m_Channel->getPaused(&isPaused);
	m_SoundList[soundName.data()].m_Channel->setPaused(!isPaused);
}

void SoundManager::Volume(const string_view& soundName, const float& value)
{
	m_SoundList[soundName.data()].operVolume(value);
}

void SoundManager::setMasterVolume(const float& value)
{
	for (auto& iter : m_SoundList)
	{
		iter.second.setVolume(value);
	}
}


bool SoundManager::Init() noexcept
{
	FMOD::System_Create(&m_pSystem);
	//		       ä�μ�, �Ӽ�
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_curBGM = nullptr;
	return true;
}

bool SoundManager::Frame() noexcept
{
	m_pSystem->update();
	
	// ���� ����
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

bool SoundManager::Render() noexcept
{
	return true;
}

bool SoundManager::Release() noexcept
{
	// ���� ó��
	for (auto& iter : m_SoundList)
	{
		iter.second.m_Sound->release();
	}
	m_pSystem->close();
	m_pSystem->release();
	return true;
}
