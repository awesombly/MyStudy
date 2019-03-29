#pragma once
#include "MyProtocol.h"

class UserInfo
{
public:
	SOCKET m_socket;
	SOCKADDR_IN m_sockInfo;

	//string m_buffer;
	//UPACKET m_packet;
	string m_userName;
public:
	UserInfo() = default;
	virtual ~UserInfo() = default;

	bool operator == (const UserInfo& user)
	{
		return this == &user;
	}
	friend bool operator == (const UserInfo* pUser, const UserInfo& pUser2);
};