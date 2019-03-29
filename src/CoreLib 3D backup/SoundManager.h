#pragma once
#include "IBasis.h"
#include "ISingleton.h"
#include "MySound.h"


class SoundManager : public IBasis, public ISingleton<SoundManager>
{
private:
	friend class  ISingleton<SoundManager>;
	SoundManager() = default;
public:
	virtual ~SoundManager() = default;
public:	
	FMOD::System*			m_pSystem;
	TCHAR					m_Buffer[256];

	map<string, MySound>	m_SoundList;
	MySound*				m_curBGM;
public:
	void	Load(const string_view& soundName, const bool& isPlay = false, const int& option = FMOD_DEFAULT);
	void	setBGM(const string_view& soundName);
	void	Play(const string_view& soundName, const bool& isPlay);
	void    PlayVariation(string&& soundName, const bool& isPlay, const int& vCount);
	void	Stop(const string_view& soundName);
	void	Paused(const string_view& soundName);
	void	Volume(const string_view& soundName, const float& value);
	void	setMasterVolume(const float& value);
public:
	bool	Init()		noexcept override;
	bool	Frame()		noexcept override;
	bool	Render()	noexcept override;
	bool	Release()	noexcept override;
};