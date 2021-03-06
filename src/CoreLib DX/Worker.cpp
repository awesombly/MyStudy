#include "Worker.h"
#include "ChattingBox.h"
#include "../../include/server/MyPacket.h"
#include "../../include/server/ClientServer.h"



bool Worker::Start(const USHORT& portNum, const string_view& address, const bool& isClient)
{
	// 쓰레드 가동
	CreateThread();
	std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	return true;
	portNum; address; isClient;
}

bool Worker::Run()
{
	{
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> 연결 되었습니다.\n";
	}
	while (m_isCreated)
	{
		{
			std::lock_guard<std::mutex> guarder(m_Mutex);
			while (!ClientServer::Instance->m_PacketPool.empty())
			{
				ProcessPacket(ClientServer::Instance->m_PacketPool.front());
			}
		}
		std::this_thread::yield();
	}
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	closesocket(m_ListenSocket);
	return true;
}

void Worker::ProcessPacket(UPACKET* pPacket)
{
	switch (pPacket->header.type)
	{
	case EPacket::CHAT_MSG:
	{
	}	break;
	default:
	{

	}	break;
	}
	ChattingBox::m_msgList.emplace_back(Server::CharToWChar(pPacket->msg));

	ClientServer::Instance->m_PacketPool.pop();
	ClientServer::Instance->m_DisablePacketPool.push(pPacket);
}