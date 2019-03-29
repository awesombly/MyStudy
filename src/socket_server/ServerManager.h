#pragma once
#include "ISingleton.h"
#include "MyPacket.h"
#include "ServerManager.h"

class ServerManager : public ISingleton<ServerManager>
{
protected:
	static list<UserInfo*>  m_UserList;				// ������ ����Ʈ
	static list<UserInfo*>	m_AcceptUserList;		// ���� ������ ����
	static list<UserInfo*>  m_DeleteUserList;		// ���� ���� ����

	queue<MyPacket*> m_PacketPool;			// ó���� ��Ŷ
	stack<MyPacket*> m_DisablePacketPool;	// ��Ŷ �޸� Ǯ

	CRITICAL_SECTION m_CriticalSection;
public:
	static unsigned int WINAPI AcceptThread(LPVOID arg);							// Ŭ�� ���� ó��
	static unsigned int WINAPI RecvThread(LPVOID agr);								// ��Ŷ ���ú� ó��
	static unsigned int WINAPI SendThread(LPVOID agr);								// ��Ŷ ���� ó��

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