#pragma once
#include "ISSingleton.h"
#include "header.h"

class UserInfo;
class MyPacket;

class ServerManager : public ISSingleton<ServerManager>
{
protected:
	queue<MyPacket*> m_PacketPool;				// 처리할 패킷
	stack<MyPacket*> m_DisablePacketPool;		// 패킷 메모리 풀

	list<UserInfo*>  m_UserList;				// 실유저 리스트
	list<UserInfo*>	 m_AcceptUserList;			// 서버 접속한 유저
public:
	stack<UserInfo*> m_DisableUserPool;			// 유저 메모리 풀
	HANDLE			 m_hIOCP = nullptr;			// IOCP 핸들
	//list<UserInfo*>  m_DeleteUserList;		// 삭제 예정 유저
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