#pragma once
#include "ISSingleton.h"
#include "ServerBasic.h"
#include "MyProtocol.h"

class ClientServer : public ISSingleton<ClientServer>
{
protected:
	list<ServerBasic*> m_ServerList;

	string m_IpAddress;
	USHORT m_PortNumber;
public:
	queue<UPACKET*> m_PacketPool;				// 처리할 패킷
	queue<UPACKET*> m_SendPacketPool;			// 서버에 전용할 패킷
	stack<UPACKET*> m_DisablePacketPool;		// 패킷 메모리 풀
	SOCKET m_ClientSock = 0;
public:
	void Init(const USHORT& portNumber, const string_view& ipAddr);
	UPACKET* TakePacket();

	void PushServer(ServerBasic* server, const bool& isClient = false);
	void TerminateServer();
private:
	friend class ISSingleton<ClientServer>;
	ClientServer() = default;
public:
	virtual ~ClientServer() = default;
};

