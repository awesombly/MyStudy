#include "Sender.h"
#include "UserInfo.h"
#include "MyPacket.h"



bool Sender::Run()
{
	{
		//Synchronize sync(&m_CriSection);
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> 연결 되었습니다.\n";
	}
	while (m_isCreated)
	{
		{
			//Synchronize sync(&m_CriSection);					// 동기화 객체 선언
			std::lock_guard<std::mutex> guarder(m_Mutex);
			while (!m_serverManager.m_PacketPool.empty())
			{
				ProcessPacket(m_serverManager.m_PacketPool.front());
			}
		}
		//Sleep(1);
		std::this_thread::yield();
	}
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	closesocket(m_ListenSocket);
	return true;
}

void Sender::ProcessPacket(MyPacket* pPacket)
{
	m_serverManager.m_PacketPool.pop();
	// 자기한테만
	if (pPacket->m_uPacket.header.type & EPacket::RANGE_SELF)
	{
		pPacket->m_uPacket.header.type &= ~EPacket::RANGE_SELF;
		SendPacket(*pPacket->m_pMyUser, *pPacket);
	}
	else
	{
		// 자신 포함 여부
		if (pPacket->m_uPacket.header.type & EPacket::RANGE_NOTSELF)
		{
			pPacket->m_uPacket.header.type &= ~EPacket::RANGE_NOTSELF;
			BroadcastMessage(*pPacket, false);
		}
		else
			BroadcastMessage(*pPacket, true);
	}
	m_serverManager.m_DisablePacketPool.push(pPacket);
}

void Sender::SendPacket(const UserInfo& target, const MyPacket& packet)
{
	int sendByte = 0;
	while (sendByte < packet.m_uPacket.header.length)
	{
		sendByte += send(target.m_socket, (char*)&packet.m_uPacket + sendByte, packet.m_uPacket.header.length - sendByte, 0);
		if(ByteErrorCheck(sendByte))
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Send Error");
			m_serverManager.DeleteUser(const_cast<UserInfo*>(&target));				// 딜리트->브로드캐스트 반복
			return;
		}
	}
}

void Sender::BroadcastMessage(const MyPacket& packet, const bool& sendSelf)
{
	// 브로드캐스팅
	for (auto& iter : m_serverManager.m_UserList)
	{
		if (!sendSelf&& &iter->m_socket == &packet.m_pMyUser->m_socket)
			continue;
		SendPacket(*iter, packet);
	}
}