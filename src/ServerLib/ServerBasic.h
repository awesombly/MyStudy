#pragma once
#include "header.h"
#include "ServerManager.h"
#include <mutex>
#include <thread>

// ���� ���� Ŭ����(����ȭ��)
//class SyncParent
//{
//protected:
//	static CRITICAL_SECTION m_CriSection;
//public:
//	SyncParent();
//	virtual ~SyncParent();
//};


// �⺻ ���� Ŭ����
class ServerBasic //: public SyncParent
{
protected:
	ServerManager& m_serverManager = ServerManager::GetInstance();
	static std::mutex  m_Mutex;
	SOCKET		m_ListenSocket;
	//UINT		m_ThreadID;
	
	static UINT WINAPI Runner(LPVOID arg);
	void CreateThread();
	virtual bool Run() = 0;
public:
	//uintptr_t	m_hThread;
	std::thread m_Thread;
	bool m_isCreated = false;
public:
	bool ByteErrorCheck(const int& byte);
	virtual bool Start(const USHORT& portNum, const string_view& address, const bool& isClient = false);

	ServerBasic() = default;
	virtual ~ServerBasic();
};



// ��ũ�� Ŭ����
//class Synchronize 
//{
//protected:
//	CRITICAL_SECTION* m_pCriSection;
//public:
//	Synchronize(CRITICAL_SECTION* criSection);
//	virtual ~Synchronize();
//};