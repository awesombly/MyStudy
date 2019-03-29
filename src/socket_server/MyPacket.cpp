#include "stdafx.h"
#include "MyPacket.h"


MyPacket::MyPacket() noexcept : m_uPacket(), m_PopPosition((char*)this), m_pMyUser(nullptr)
{
	m_PopPosition = (char*)this->m_uPacket.msg;
}


//void MyPacket::SetPacket(UserInfo* pUserInfo, const string_view& msg, const WORD msgType, const bool& printServer)
//{
//	if (msg != &pUserInfo->m_packet.msg[0])
//		ZeroMemory(&pUserInfo->m_packet, sizeof(UPACKET));
//	pUserInfo->m_packet.header.type = msgType;
//
//	static string buffer;
//	switch (msgType)
//	{
//	case CHAT_MSG:
//	{
//		if (auto length = strlen(pUserInfo->m_packet.msg); pUserInfo->m_packet.msg[length - 1] != '\n')
//			pUserInfo->m_packet.msg[length] = '\n';
//
//		buffer = pUserInfo->m_userName + " : " + msg.data();
//	}	break;
//	case PACKET_CHAT_NAME_ACK:
//	{
//		pUserInfo->m_packet.header.type = CHAT_MSG;
//		pUserInfo->m_userName = msg;
//		buffer = "["s + msg.data() + "]";
//	}	break;
//	case PACKET_SYSTEM_MSG:
//	{
//		pUserInfo->m_packet.header.type = CHAT_MSG;
//		buffer = msg;
//	}	break;
//	default:
//	{
//		stringstream ostr;
//		ostr << endl << __FUNCTION__ << " -> " << pUserInfo->m_userName << ", MSG Type Not Found!";
//		OutputDebugStringA(ostr.str().c_str());
//		buffer = msg;
//	}	break;
//	}
//	if (printServer)
//		std::cout << buffer.c_str();
//	memcpy(pUserInfo->m_packet.msg, buffer.c_str(), strlen(buffer.c_str()));
//	pUserInfo->m_packet.header.length = WORD(strlen(pUserInfo->m_packet.msg) + PACKET_HEADER_SIZE);
//}
//
//void MyPacket::SendPacket(const UserInfo& target, const UserInfo& sendInfo)
//{
//	int sendByte = 0;
//	while (sendByte < sendInfo.m_packet.header.length)
//	{
//		sendByte += send(target.m_socket, (char*)&sendInfo.m_packet + sendByte, sendInfo.m_packet.header.length - sendByte, 0);
//		if (sendByte == SOCKET_ERROR)
//		{
//			if (WSAGetLastError() != WSAEWOULDBLOCK)
//			{
//				// 에러
//				stringstream ostr;
//				ostr << endl << __FUNCTION__ << " -> Error : " << sendInfo.m_userName;
//				OutputDebugStringA(ostr.str().c_str());
//				//DeleteUser(const_cast<UserInfo*>(&target));				// 딜리트->브로드캐스트 반복
//				return;
//			}
//		}
//	}
//}
//
//void  MyPacket::PushData(char* pData, const int& size)
//{
//	
//}
//
//char* MyPacket::PopData(char* pData, const int& size)
//{
//	return nullptr;
//}

