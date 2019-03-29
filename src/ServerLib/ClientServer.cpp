#include "ClientServer.h"




void ClientServer::Init(const USHORT& portNumber, const string_view& ipAddr)
{
	m_PortNumber = portNumber;
	m_IpAddress = ipAddr.data();
}

UPACKET* ClientServer::TakePacket()
{
	UPACKET* pPacket = nullptr;
	if (m_DisablePacketPool.empty())
	{
		// 대기 풀이 비었다면 생성
		pPacket = new UPACKET();
	}
	else
	{
		// 대기 풀이 있다면 꺼내옴
		pPacket = m_DisablePacketPool.top();
		m_DisablePacketPool.pop();
		ZeroMemory(pPacket, sizeof(*pPacket));
	}
	return pPacket;
}

void ClientServer::PushServer(ServerBasic* server, const bool& isClient)
{
	if(server->Start(m_PortNumber, m_IpAddress, isClient))
		m_ServerList.push_back(server);
}

void ClientServer::TerminateServer()
{
	for (auto& iter : m_ServerList)
	{
		iter->m_isCreated = false;
	}
	for (auto& iter : m_ServerList)
	{
		if (iter->m_Thread.joinable())
			iter->m_Thread.join();
	}
}
