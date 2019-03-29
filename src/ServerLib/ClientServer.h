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
	queue<UPACKET*> m_PacketPool;				// ó���� ��Ŷ
	queue<UPACKET*> m_SendPacketPool;			// ������ ������ ��Ŷ
	stack<UPACKET*> m_DisablePacketPool;		// ��Ŷ �޸� Ǯ
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

