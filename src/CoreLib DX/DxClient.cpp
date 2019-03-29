#include "DxClient.h"
#include "Worker.h"
#include "../../include/server/ClientServer.h"


//DxClient::DxClient() //: m_MsgType(EPacket::CHAT_MSG), m_Recver(m_MsgType), m_Sender(m_MsgType)
//{}
//
//DxClient::~DxClient()
//{
//	if (m_Recver.m_Thread.joinable())
//		m_Recver.m_Thread.join();
//	if (m_Sender.m_Thread.joinable())
//		m_Sender.m_Thread.join();
//}
//
//bool DxClient::Start(const USHORT& portNum, const string_view& address)
//{
//	m_Recver.Start(portNum, address, true);
//	m_Sender.Start(portNum, address, true);
//	return true;
//}

void DxClientRecv::PushData(const int& size) noexcept
{
	//memcpy(m_packetBuffer + m_pushPos, data, size);
	m_pushPos += size;

	// ����� ���޾Ҵ��� �⺻������ 0���� ó���ǹǷ� ���� X
	m_pUPacket = (UPACKET*)m_packetBuffer;
	// ���� �����Ͱ� �ٲ� ��Ŷ ���̺��� ũ�ٸ�
	while (m_pUPacket->header.length <= m_pushPos)
	{
		// ��Ŷ �ϼ�
		m_pPacket = ClientServer::GetInstance().TakePacket();
		memcpy(m_pPacket, m_packetBuffer + m_popPos, m_pUPacket->header.length);
		// ��Ŷ ó��(+�ٹٲ�)
		if (auto length = strlen(m_pPacket->msg);  m_pPacket->msg[length - 1] != '\n')
			m_pPacket->msg[length] = '\n';
		ClientServer::Instance->m_PacketPool.push(m_pPacket);
		// ���� ���� ����
		m_popPos += m_pUPacket->header.length;				// ���� ������
		m_pushPos -= m_pUPacket->header.length;				// ��Ŷ ���̸�ŭ ����
		m_pUPacket = (UPACKET*)&m_packetBuffer[m_popPos];	// ���� ��Ŷ ����
		if (m_pushPos == 0 || m_pUPacket->header.length == 0)
			break;
	}
	// ���� ���� �����
	memcpy(m_packetBuffer, m_packetBuffer + m_popPos, MaxBufferSize - m_popPos);
	ZeroMemory(m_packetBuffer + MaxBufferSize - m_popPos, m_popPos);
	m_popPos = 0;
}

bool DxClientRecv::Run()
{
	{
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
	}
	// Ŭ�� ���� ����
	ClientServer::Instance->m_ClientSock = m_ListenSocket;

	UPACKET sendPacket = { 0, };
	std::this_thread::sleep_for(std::chrono::seconds(10));
	sendPacket.msg[0] = 'A';
	sendPacket.header.length = (WORD)(strlen(sendPacket.msg) + PACKET_HEADER_SIZE);
	sendPacket.header.type = EPacket::CHAT_NAME_ACK;
	send(m_ListenSocket, (char*)&sendPacket, sendPacket.header.length, 0);

	while (m_isCreated)
	{
		// ����->���ú�
		int receiveByte = 0;
		receiveByte = recv(m_ListenSocket, (char*)&m_packetBuffer + m_pushPos, MaxBufferSize / 2, 0);
		if (ByteErrorCheck(receiveByte))
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> DxClient Recv Error!");
			m_isCreated = false;
			break;
		}
		else
		{
			// ��Ŷ ó��
			std::lock_guard<std::mutex> guarder(m_Mutex);
			PushData(receiveByte);
		}
		std::this_thread::yield();
	}
	shutdown(m_ListenSocket, SD_SEND);
	closesocket(m_ListenSocket);
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	return false;
}

bool DxClientSend::Start(const USHORT& portNum, const string_view& address, const bool& isClient)
{
	//while (true)
	//{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (ClientServer::Instance->m_ClientSock != 0)
		{
			m_ListenSocket = ClientServer::Instance->m_ClientSock;
		//	break;
		}
	//}
	// ������ ����
	CreateThread();
	return true;
	portNum; address; isClient;
}

// ��Ŷ ��Ʈ��ȭ?
bool DxClientSend::Run()
{
	{
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
	}
	//UPACKET sendPacket = { 0, };

	/*std::this_thread::sleep_for(std::chrono::seconds(10));
	sendPacket.msg[0] = 'A';
	sendPacket.header.length = (WORD)(strlen(sendPacket.msg) + PACKET_HEADER_SIZE);
	sendPacket.header.type = EPacket::CHAT_NAME_ACK;
	send(m_ListenSocket, (char*)&sendPacket, sendPacket.header.length, 0);*/

	UPACKET* pPacket = nullptr;
	while (m_isCreated)
	{
		// ����->����
		//sendPacket = { 0, };
		//sendPacket.msg[0] = 'A';
		//fgets(sendPacket.msg, sizeof(sendPacket.msg), stdin);
		{
			std::lock_guard<std::mutex> guarder(m_Mutex);
			while (!ClientServer::Instance->m_SendPacketPool.empty())
			{
				pPacket = ClientServer::Instance->m_SendPacketPool.front();
				int sendByte = send(m_ListenSocket, (char*)pPacket, pPacket->header.length, 0);
				if (ByteErrorCheck(sendByte))
				{
					Server::ErrorMessage(""s + __FUNCTION__ + " -> Server DxClient Send Error!");
					break;
				}
				ClientServer::Instance->m_SendPacketPool.pop();
				ClientServer::Instance->m_DisablePacketPool.push(pPacket);
			}
		}
		std::this_thread::yield();
	}
	shutdown(m_ListenSocket, SD_SEND);
	closesocket(m_ListenSocket);
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	return false;
}