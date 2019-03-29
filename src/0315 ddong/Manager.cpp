#include "header.h"

Manager* Manager::instance;


Manager::Manager()
{
	m_pBuffer = new Buffer();
	m_pPlayer = new Player();
	
	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
		{
			if (h == HEIGHT - 1 || w == 0 || w == WIDTH - 1)
			{
				m_blockList.emplace_back(Block(w, h));
			}
		}
	}
	
	m_isPlaying = true;
}

Manager::~Manager()	{}

Manager* Manager::getInstance()
{
	if (instance == nullptr)	instance = new Manager();
	return instance;
}


void Manager::Draw() const 
{
	m_pBuffer->BufferClear();
	
	for (auto& list : m_blockList)
	{
		list.Draw(*m_pBuffer);
	}
	for (auto& list : m_dungList)
	{
		list.Draw(*m_pBuffer);
	}
	m_pPlayer->Draw(*m_pBuffer);
	
	if(!m_isPlaying)	
		m_pBuffer->BufferWrite("Game Over~!", 0, HEIGHT + 4);

	m_pBuffer->BufferFlipping();
}

void Manager::KeyCheck()
{
	while (m_isPlaying)
	{
		switch (_getch())
		{ 
		case LEFT:
			m_pPlayer->Move(-1, 0);
			ColisionCheck();
			break;
		case RIGHT:
			m_pPlayer->Move(1, 0);
			ColisionCheck();
			break;
		}
		//Draw();
	}
}

void Manager::Process() {
	int timer = 250;				// ���� ����
	time_t start = 0, end = 0;
	int objCounter = 0;
	while (m_isPlaying)
	{
		end = clock();
		if (end - start > timer)
		{
			start = clock();
			if (timer > 20) timer -= 2;

			// list<T>::iterator i = myList.begin();
			for(auto& iter = m_dungList.begin(); iter != m_dungList.end(); iter++)
			{
				iter->Move(0, 1);
				if (iter->ColisionCheck())				// ��vs�� �浹
				{
					iter = m_dungList.erase(iter);		// list�� erase�� �ش� ���Ұ� ������ ���� ���� ��ġ�� �޴���, iter++�� ����� �Ѵ�
				}
			}
			ColisionCheck();							// ��vs�÷��̾� �浹

			if (++objCounter >= 2)
			{
				objCounter = 0;
				CreateObject();
			}
			Draw();
		}
	}
}

void Manager::ColisionCheck() {
	for (auto& iter = m_dungList.begin(); iter != m_dungList.end(); iter++)
	{
		if (*m_pPlayer == iter->getPosition())				// ��vs�÷��̾� �浹��
		{
			iter = m_dungList.erase(iter);
			if (!m_pPlayer->operLife(-1))	GameOver();		// ü�� ���� �� ���ӿ��� üũ
		}
	}
}

void Manager::CreateObject() {
	m_dungList.emplace_back(Dung((rand() % (WIDTH - 2)) + 1, 0));
}

void Manager::GameOver()
{
	m_isPlaying = false;
}
