#pragma once
#include "ISSingleton.h"
#include "header.h"

class UserInfo;
class MyPacket;

class ServerManager : public ISSingleton<ServerManager>
{
protected:
	queue<MyPacket*> m_PacketPool;				// ó���� ��Ŷ
	stack<MyPacket*> m_DisablePacketPool;		// ��Ŷ �޸� Ǯ

	list<UserInfo*>  m_UserList;				// ������ ����Ʈ
	list<UserInfo*>	 m_AcceptUserList;			// ���� ������ ����
public:
	stack<UserInfo*> m_DisableUserPool;			// ���� �޸� Ǯ
	HANDLE			 m_hIOCP = nullptr;			// IOCP �ڵ�
	//list<UserInfo*>  m_DeleteUserList;		// ���� ���� ����
public:
	MyPacket* TakePacket();
	UserInfo* TakeUserInfo();
	void DeleteUser(UserInfo* pDelUser);
	void SetPacket(MyPacket* pPacket, UserInfo* pUserInfo, const string_view& msg, const WORD& msgType, const WORD& flag = 0, const bool& printServer = true);
	void SetPacket(MyPacket* pPacket, UserInfo* pUserInfo, const WORD& flag = 0, const bool& printServer = true);
private:
	friend class ISSingleton<ServerManager>;
	friend class Acceptor;
	friend class Sender;
	friend class Receiver;
	ServerManager();
public:
	virtual ~ServerManager() = default;
};