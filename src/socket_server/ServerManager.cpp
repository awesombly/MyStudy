#include "stdafx.h"
#include "ServerManager.h"

list<UserInfo*> ServerManager::m_UserList;
list<UserInfo*> ServerManager::m_AcceptUserList;
list<UserInfo*> ServerManager::m_DeleteUserList;

ServerManager::ServerManager()
{
	InitializeCriticalSection(&m_CriticalSection);
}

unsigned int WINAPI ServerManager::AcceptThread(LPVOID arg)
{
	SOCKET listenSock = (SOCKET)arg;

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLength = 0;

	while (true)
	{
		EnterCriticalSection(&Instance->m_CriticalSection);
		addrLength = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLength);
		if (clientSock == SOCKET_ERROR)
		{
			// 논블록일시, 접속이 없다면 에러 반환
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				// ~~에러~~
				Instance->ErrorMessage(""s + __FUNCTION__ + " -> Client Accept Error!");
				continue;
			}
			// 입력 없음
		}
		else
		{
			// 접속시 유저 등록
			UserInfo* pUser = new UserInfo();
			pUser->m_socket = clientSock;
			pUser->m_sockInfo = clientAddr;
			m_UserList.push_back(pUser);
			std::cout << "~클라이언트 접속~ IP : " << inet_ntoa(pUser->m_sockInfo.sin_addr) << ", Port : " << ntohs(pUser->m_sockInfo.sin_port) << endl;
			Instance->SetPacket(Instance->TakePacket(), pUser, "\nㅎㅇㅇ~! 아이디를 입력해 주세용.\n", PACKET_CHAT_NAME_REQ, false, true);
		}
		LeaveCriticalSection(&Instance->m_CriticalSection);
		Sleep(1);
	}
	return 0;
}

unsigned int WINAPI ServerManager::SendThread(LPVOID agr)
{
	while (true)
	{
		EnterCriticalSection(&Instance->m_CriticalSection);
		if (Instance->m_PacketPool.empty())
		{
			LeaveCriticalSection(&Instance->m_CriticalSection);
			Sleep(1);
			continue;
		}
		auto& pPacket = Instance->m_PacketPool.front();
		Instance->m_PacketPool.pop();

		// 자기한테만
		auto i = pPacket->m_uPacket.header.type & PACKET_RANGE_SELF;
		if (pPacket->m_uPacket.header.type & PACKET_RANGE_SELF)
		{
			pPacket->m_uPacket.header.type &= ~PACKET_RANGE_SELF;
			Instance->SendPacket(*pPacket->m_pMyUser, *pPacket);
		}
		else
		{
			// 자신 포함 여부
			if (pPacket->m_uPacket.header.type & PACKET_RANGE_NOTSELF)
			{
				pPacket->m_uPacket.header.type &= ~PACKET_RANGE_NOTSELF;
				Instance->BroadcastMessage(*pPacket, false);
			}
			else
				Instance->BroadcastMessage(*pPacket, true);
		}
		Instance->m_DisablePacketPool.push(pPacket);
		LeaveCriticalSection(&Instance->m_CriticalSection);
		Sleep(1);
	}

	return 1;
}

unsigned int WINAPI ServerManager::RecvThread(LPVOID agr)
{
	MyPacket* pRecvPacket = nullptr;
	pRecvPacket = Instance->TakePacket();
	while (true)
	{
		EnterCriticalSection(&Instance->m_CriticalSection);
		// 서버->리시브
		for (auto& pRecvIter : m_UserList)
		{
			// 헤더 만들기
			int receiveByte = 0;
			while (receiveByte < PACKET_HEADER_SIZE)
			{
				// 패킷 헤더
				receiveByte += recv(pRecvIter->m_socket, (char*)&pRecvPacket->m_uPacket + receiveByte, PACKET_HEADER_SIZE - receiveByte, 0);
				if (receiveByte == SOCKET_ERROR || receiveByte == 0)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						// 에러 -> 유저 내보냄
						Instance->ErrorMessage(""s + __FUNCTION__ + " -> User Exit : " + pRecvIter->m_userName.c_str());
						char ip[22];
						std::cout << "\n!~유저 강종~! Ip = " << inet_ntop(AF_INET, &(pRecvIter->m_sockInfo.sin_addr), ip, INET_ADDRSTRLEN)
								  << ", Port = " << ntohs(pRecvIter->m_sockInfo.sin_port) << endl;
						Instance->DeleteUser(pRecvIter);
						break;
					}
					// 입력 없음
					receiveByte = 0;
					break;
				}
				// 패킷 메세지
				while (receiveByte < pRecvPacket->m_uPacket.header.length)
				{
					receiveByte += recv(pRecvIter->m_socket, (char*)&pRecvPacket->m_uPacket + receiveByte, pRecvPacket->m_uPacket.header.length - receiveByte, 0);
					if (receiveByte == SOCKET_ERROR || receiveByte == 0)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							// 에러 -> 유저 내보냄
							Instance->ErrorMessage(""s + __FUNCTION__ + " -> User Exit Error!  : " + pRecvIter->m_userName.c_str());
							char ip[22];
							std::cout << "\n!~유저 강종(메세지)~! Ip = " << inet_ntop(AF_INET, &(pRecvIter->m_sockInfo.sin_addr), ip, INET_ADDRSTRLEN)
									  << ", Port = " << ntohs(pRecvIter->m_sockInfo.sin_port) << endl;
							Instance->DeleteUser(pRecvIter);
							break;
						}
						// 입력 없음
						receiveByte = 0;
						break;
					}
				}
			}
			if (receiveByte == 0)
				continue;
			// 패킷 처리
			if (receiveByte == pRecvPacket->m_uPacket.header.length)
			{
				// 리시브 됐다면 샌드
				if (auto length = strlen(pRecvPacket->m_uPacket.msg); pRecvPacket->m_uPacket.msg[length - 1] == '\n')
						pRecvPacket->m_uPacket.msg[length - 1] = '\0';
				Instance->SetPacket(pRecvPacket, pRecvIter, pRecvPacket->m_uPacket.msg, pRecvPacket->m_uPacket.header.type, true);
				pRecvPacket = Instance->TakePacket();
			}
		}
		// 실질적 유저 삭제
		if (!m_DeleteUserList.empty())
		{
			for (auto iter = m_DeleteUserList.begin(); iter != m_DeleteUserList.end();)
			{
				auto delIter = find(m_UserList.begin(), m_UserList.end(), **iter);
				m_UserList.erase(delIter);
				m_DeleteUserList.erase(iter);
				iter = m_DeleteUserList.begin();
			}
		}
		LeaveCriticalSection(&Instance->m_CriticalSection);
		Sleep(1);
	}
	return 0;
}

MyPacket* ServerManager::TakePacket()
{
	MyPacket* pPacket = nullptr;
	if (m_DisablePacketPool.empty())
	{
		// 대기 풀이 비었다면 생성
		pPacket = new MyPacket();
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

void ServerManager::SetPacket(MyPacket* pPacket , UserInfo* pUserInfo, const string_view& msg, const WORD msgType, const bool& printServer, const bool& rangeSelf)
{
	pPacket->m_uPacket.header.type = msgType;
	pPacket->m_pMyUser = pUserInfo;

	static string buffer;
	switch (msgType)
	{
	case CHAT_MSG:
	{
		buffer = pUserInfo->m_userName + " : " + msg.data();
		pPacket->m_uPacket.header.type |= PACKET_RANGE_NOTSELF;
	}	break;
	case PACKET_CHAT_NAME_ACK:
	{
		pPacket->m_uPacket.header.type = CHAT_MSG;
		pUserInfo->m_userName = msg;
		buffer = "["s + msg.data() + "]" + "님이 입장하셨습니다.\n";
	}	break;
	case PACKET_CHAT_NAME_REQ:
	{
		buffer = msg;
	}	break;
	case PACKET_SYSTEM_MSG:
	{
		pPacket->m_uPacket.header.type = CHAT_MSG;
		buffer = msg;
	}	break;
	default:
	{
		ErrorMessage(""s + __FUNCTION__ + " -> " + pUserInfo->m_userName + " : MSG Type Not Found!");
		buffer = msg;
	}	break;
	}
	// 메세지, 길이
	memcpy(pPacket->m_uPacket.msg, buffer.c_str(), strlen(buffer.c_str()));
	if (auto length = strlen(pPacket->m_uPacket.msg); 
		pPacket->m_uPacket.msg[length - 1] != '\n')
		pPacket->m_uPacket.msg[length]		= '\n';
	pPacket->m_uPacket.header.length = WORD(strlen(pPacket->m_uPacket.msg) + PACKET_HEADER_SIZE);

	if (printServer)	cout << pPacket->m_uPacket.msg;
	if (rangeSelf)		pPacket->m_uPacket.header.type |= PACKET_RANGE_SELF;
	// 패킷풀에 패킷 추가
	m_PacketPool.push(pPacket);
}

void ServerManager::SendPacket(const UserInfo& target, const MyPacket& packet)
{
	int sendByte = 0;
	while (sendByte < packet.m_uPacket.header.length)
	{
		sendByte += send(target.m_socket, (char*)&packet.m_uPacket + sendByte, packet.m_uPacket.header.length - sendByte, 0);
		if (sendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				ErrorMessage(""s + __FUNCTION__ + " -> Send Error");
				DeleteUser(const_cast<UserInfo*>(&target));				// 딜리트->브로드캐스트 반복
				return;
			}
		}
	}
}

void ServerManager::BroadcastMessage(const MyPacket& packet, const bool& sendSelf)
{
	// 브로드캐스팅
	for (auto& iter : m_UserList)
	{
		if (&iter->m_socket == &packet.m_pMyUser->m_socket && !sendSelf)
			continue;
		SendPacket(*iter, packet);
	}
}

void ServerManager::DeleteUser(UserInfo* pDelUser)
{
	auto iter = find(m_UserList.begin(), m_UserList.end(), pDelUser);
	if (iter == m_UserList.end())
	{
		ErrorMessage(""s + __FUNCTION__ + " -> " + pDelUser->m_userName.c_str() + " : UserInfo Not Found!");
		return;
	}
	auto checkIter = find(m_DeleteUserList.begin(), m_DeleteUserList.end(), pDelUser);
	if (checkIter != m_DeleteUserList.end())
		return;
	SetPacket(TakePacket(), pDelUser, (pDelUser->m_userName + "님이 나가셨습니다.\n"), PACKET_SYSTEM_MSG, true);
	closesocket(pDelUser->m_socket);
	m_DeleteUserList.push_back((*iter));
	//m_UserList.erase(iter);
}


void ServerManager::ErrorMessage(const string_view& msg)
{
	// 에러
	static stringstream ostr;
	ostr.str("");
	ostr << endl << msg << " ";
	OutputDebugStringA(ostr.str().c_str());
}