#include "Client.h"
#include "MyProtocol.h"



Client::Client() : m_MsgType(EPacket::CHAT_MSG), m_Recver(m_MsgType), m_Sender(m_MsgType)
{}


Client::~Client()
{
	//m_Recver.m_Thread.native_handle();
	//m_Sender.m_Thread.native_handle();
	if (m_Recver.m_Thread.joinable())
		m_Recver.m_Thread.join();
	if (m_Sender.m_Thread.joinable())
		m_Sender.m_Thread.join();
}

bool Client::Start(const USHORT& portNum, const string_view& address)
{
	m_Recver.Start(portNum, address, true);
	m_Sender.Start(portNum, address, true);
	return true;
}

bool ClientRecv::Run()
{
	{
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
	}
	UPACKET recvPacket;

	while (m_isCreated)
	{
		{
			// ����->���ú�
			recvPacket = { 0, };

			// ��� �����
			int receiveByte = 0;
			while (receiveByte < PACKET_HEADER_SIZE)
			{
				receiveByte += recv(m_ListenSocket, (char*)&recvPacket + receiveByte, PACKET_HEADER_SIZE - receiveByte, 0);
				if (ByteErrorCheck(receiveByte))
				{
					Server::ErrorMessage(""s + __FUNCTION__ + " -> Server Client Header Recv Error!");
					m_isCreated = false;
					break;
				}
			}
			// �޼��� �����
			while (receiveByte < recvPacket.header.length)
			{
				receiveByte += recv(m_ListenSocket, (char*)&recvPacket + receiveByte, recvPacket.header.length - receiveByte, 0);
				if (receiveByte == SOCKET_ERROR || receiveByte == 0)
				{
					Server::ErrorMessage(""s + __FUNCTION__ + " -> Server Client Message Recv Error!");
					m_isCreated = false;
					break;
				}
			}
			std::lock_guard<std::mutex> guarder(m_Mutex);
			// ��Ŷ �ϼ� ó��
			if (receiveByte == recvPacket.header.length)
			{
				switch (recvPacket.header.type)
				{
				case EPacket::CHAT_MSG:
				{
					//g_msgType = CHAT_MSG;
				}	break;
				case  EPacket::CHAT_NAME_REQ:
				{
					m_MsgType = EPacket::CHAT_NAME_ACK;
				}	break;
				default:
				{
					Server::ErrorMessage(""s + __FUNCTION__ + " -> " + std::to_string(recvPacket.header.type) + ", MSG Type Not Found!");
				}	break;
				}
				// ���� ������ ���, �ٹٲ� ������ �־���
				if (auto length = strlen(recvPacket.msg);  recvPacket.msg[length - 1] != '\n')
					recvPacket.msg[length] = '\n';
				cout << "~ ServerMessage ~ : " << recvPacket.msg;
			}
		}
		std::this_thread::yield();
	}
	shutdown(m_ListenSocket, SD_SEND);
	closesocket(m_ListenSocket);
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	return false;
}

// ��Ŷ ��Ʈ��ȭ?
bool ClientSend::Run()
{
	{
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
	}
	UPACKET sendPacket;
	while (m_isCreated)
	{
		// ����->����
		sendPacket = { 0, };
		sendPacket.msg[0] = 'A';
		//fgets(sendPacket.msg, sizeof(sendPacket.msg), stdin);
		{
			std::lock_guard<std::mutex> guarder(m_Mutex);

			if (sendPacket.msg[0] == '`')
				break;
			sendPacket.header.length = (WORD)(strlen(sendPacket.msg) + PACKET_HEADER_SIZE);
			sendPacket.header.type = m_MsgType;

			// ����->����
			int sendByte = send(m_ListenSocket, (char*)&sendPacket, sendPacket.header.length, 0);
			if (ByteErrorCheck(sendByte))
			{
				Server::ErrorMessage(""s + __FUNCTION__ + " -> Server Client Send Error!");
				break;
			}
			//cout << sendByte << " ����Ʈ�� �����ߴ�.";
			m_MsgType = EPacket::CHAT_MSG;
		}
		std::this_thread::yield();
	}
	shutdown(m_ListenSocket, SD_SEND);
	closesocket(m_ListenSocket);
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	return false;
}