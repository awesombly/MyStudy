#pragma once
#include "ISingleton.h"
#include "MyPacket.h"
#include "ServerManager.h"

class ServerManager : public ISingleton<ServerManager>
{
protected:
	static list<UserInfo*>  m_UserList;				// 실유저 리스트
	static list<UserInfo*>	m_AcceptUserList;		// 서버 접속한 유저
	static list<UserInfo*>  m_DeleteUserList;		// 삭제 예정 유저

	queue<MyPacket*> m_PacketPool;			// 처리할 패킷
	stack<MyPacket*> m_DisablePacketPool;	// 패킷 메모리 풀

	CRITICAL_SECTION m_CriticalSection;
public:
	static unsigned int WINAPI AcceptThread(LPVOID arg);							// 클라 접속 처리
	static unsigned int WINAPI RecvThread(LPVOID agr);								// 패킷 리시브 처리
	static unsigned int WINAPI SendThread(LPVOID agr);								// 패킷 샌드 처리

	void BroadcastMessage(const MyPacket& packet, const bool& sendSelf);
	void DeleteUser(UserInfo* pDelUser);

	MyPacket* TakePacket();
	void SetPacket(MyPacket* pPacket, UserInfo* pUserInfo, const string_view& msg, const WORD msgType, const bool& printServer, const bool& rangeSelf = false);
	void SendPacket(const UserInfo& target, const MyPacket& packet);

	void ErrorMessage(const string_view& msg);
private:
	friend class ISingleton<ServerManager>;
	ServerManager();
public:
	virtual ~ServerManager() = default;
};