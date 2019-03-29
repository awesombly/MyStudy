#pragma once
#include "Basis.h"
#include "Singleton.h"
#include "MySound.h"


class SoundManager : public Basis, public Singleton<SoundManager>
{
private:
	friend class  Singleton<SoundManager>;
	SoundManager() = default;
public:
	virtual ~SoundManager() = default;
public:	
	FMOD::System*			m_pSystem;
	TCHAR					m_Buffer[256];

	map<string, MySound>	m_SoundList;
	MySound*				m_curBGM;
public:
	void	Load(const string& soundName, const bool& isPlay = false, const int& option = FMOD_DEFAULT);
	void	setBGM(const string& soundName);
	void	Play(const string& soundName, const bool& isPlay);
	void    PlayVariation(string&& soundName, const bool& isPlay, const int& vCount);
	void	Stop(const string& soundName);
	void	Paused(const string& soundName);
	void	Volume(const string& soundName, const float& value);
public:
	bool	Init()		override;
	bool	Frame()		override;
	bool	Render()	override;
	bool	Release()	override;
};

