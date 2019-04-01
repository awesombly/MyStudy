#pragma once
//#include "header.h"
#include <list>

using namespace std;

class Buffer;
class Player;
class Object;
class Dung;

class Manager
{
private:
	Manager();
public:
	~Manager();
private:
	static Manager* instance;
	Buffer* m_pBuffer;
	Player* m_pPlayer;
	std::list<Block> m_blockList;
	std::list<Dung> m_dungList;
	bool m_isPlaying;
public:
	static Manager* getInstance();
	void Draw() const;
	void KeyCheck();
	void Process();
	void ColisionCheck();
	void CreateObject();
	void GameOver();
};