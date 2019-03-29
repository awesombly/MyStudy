#pragma once
#include "ServerBasic.h"
#include "MyProtocol.h"
#include "PacketStream.h"
//#include "ServerManager.h"

class UserInfo;

class UserThread : public ServerBasic
{
protected:
	virtual bool Run() override;
public:
	UserInfo * m_pUser;

	UserThread() = default;
	~UserThread() = default;
};


class UserInfo
{
public:
	SOCKET			m_socket;
	SOCKADDR_IN		m_sockInfo;
	string			m_userName;

	PacketStream	m_packetStream;
	//UserThread	m_recvThread;
public:
	void Init();
	void Release();
	void RecvAsync();

	UserInfo();
	virtual ~UserInfo() = default;
	
	bool operator == (const UserInfo& user)
	{
		return this == &user;
	}
	friend bool operator == (const UserInfo* pUser, const UserInfo& pUser2);
};