// socket_server.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerManager.h"


//list<UserInfo> g_Userlist;
//list<UserInfo*> g_UserDeleteList;
//
//CRITICAL_SECTION g_CriticalSection;
//
//void DeleteUser(UserInfo* pDelUser);
//void BroadcastMessage(const UserInfo& userInfo, const bool& sendSelf);
//void SetPacket(UserInfo* pUserInfo, const string_view& msg, const WORD msgType, const bool& printServer);
//void SendPacket(const UserInfo& target, const UserInfo& sendInfo);
//
//
//void SetPacket(UserInfo* pUserInfo, const string_view& msg, const WORD msgType, const bool& printServer)
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
//		pUserInfo->m_packet.header.type =  CHAT_MSG;
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
//void SendPacket(const UserInfo& target, const UserInfo& sendInfo)
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
//void BroadcastMessage(const UserInfo& userInfo, const bool& sendSelf)
//{
//	// 브로드캐스팅
//	for (auto& iter : g_Userlist)
//	{
//		if (iter == userInfo && !sendSelf)
//			continue;
//		SendPacket(iter, userInfo);
//	}
//}
//
//void DeleteUser(UserInfo* pDelUser)
//{
//	auto iter = find(g_Userlist.begin(), g_Userlist.end(), *pDelUser);
//	if (iter == g_Userlist.end())
//	{
//		wstringstream ostr;
//		ostr << endl << L"UserInfo Not Found!";
//		OutputDebugString(ostr.str().c_str());
//		return;
//	}
//	auto checkIter = find(g_UserDeleteList.begin(), g_UserDeleteList.end(), pDelUser);
//	if (checkIter != g_UserDeleteList.end())
//		return;
//	SetPacket(pDelUser, (pDelUser->m_userName + "님이 나가셨습니다.\n").c_str(), PACKET_SYSTEM_MSG, true);
//	BroadcastMessage(*pDelUser, true);
//	closesocket(pDelUser->m_socket);
//	g_UserDeleteList.push_back(&(*iter));
//	//g_Userlist.erase(iter);
//}

//unsigned int WINAPI AcceptThread(LPVOID arg)
//{
//	SOCKET listenSock = (SOCKET)arg;
//
//	SOCKET clientSock;
//	SOCKADDR_IN clientAddr;
//	int addrLength = 0;
//
//	while (true)
//	{
//		EnterCriticalSection(&g_CriticalSection);
//		addrLength = sizeof(clientAddr);
//		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLength);
//		if (clientSock == SOCKET_ERROR)
//		{
//			// 논블록일시, 접속이 없다면 에러 반환
//			if (WSAGetLastError() != WSAEWOULDBLOCK)
//			{
//				// ~~에러~~
//				wstringstream ostr;
//				ostr << endl << L"Client Accept Error!";
//				OutputDebugString(ostr.str().c_str());
//				continue;
//			}
//			// 입력 없음
//		}
//		else
//		{
//			// 접속시 유저 등록
//			UserInfo user;
//			user.m_socket = clientSock;
//			user.m_sockInfo = clientAddr;
//			std::cout << "클라이언트 접속~ IP : " << inet_ntoa(user.m_sockInfo.sin_addr)
//					  << ", Port : " << ntohs(user.m_sockInfo.sin_port) << endl;
//			g_Userlist.push_back(user);
//
//			SetPacket(&user, "\nㅎㅇㅇ~! 아이디를 입력해 주세용.\n", PACKET_SYSTEM_MSG, false);
//			SendPacket(user, user);
//		}
//		LeaveCriticalSection(&g_CriticalSection);
//		Sleep(1);
//	}
//	return 0;
//}
//
//unsigned int WINAPI RecvSendThread(LPVOID agr)
//{
//	//UPACKET recvPacket;
//	MyPacket packet;
//
//	char* abc = new char[10];
//	packet << "asddsa";
//	packet >> abc;
//
//	const char* aaa;
//	packet << aaa;
//	packet << abc;
//	strcpy_s(abc,10, "qqaaa");
//	char bbb[10];
//	packet >> bbb;
//	//packet << 5 << 10 << 20 << 30 << 5555;
//	//int a = 10;
//	//UserInfo info;
//	//char aaa[5] = "asd";
//	//string aaa;
//	//float zz = 0.0f;;
//	//info.m_userName = "아야어여오요우유";
//	//packet << 10.0f  << info.m_userName;
//	//info.m_userName = "asddatutuyututuyutytuts";
//	//packet >> &zz >> &info.m_userName;
//	//packet << a << 10.0f << "asdads";
//	while (true)
//	{
//		EnterCriticalSection(&g_CriticalSection);
//		// 서버-클라이언트 구동부
//		for (auto& recvIter : g_Userlist)
//		{
//			// 서버->리시브
//			ZeroMemory(&recvIter.m_packet, sizeof(UPACKET));
//			// 헤더 만들기
//			int receiveByte = 0;
//			while (receiveByte < PACKET_HEADER_SIZE)
//			{
//				// 패킷 헤더
//				receiveByte += recv(recvIter.m_socket, (char*)&recvIter.m_packet + receiveByte, PACKET_HEADER_SIZE - receiveByte, 0);
//				if (receiveByte == SOCKET_ERROR || receiveByte == 0)
//				{
//					if (WSAGetLastError() != WSAEWOULDBLOCK)
//					{
//						// 에러 -> 유저 내보냄
//						wstringstream ostr;
//						ostr << endl << L"User Receive Error!";
//						OutputDebugString(ostr.str().c_str());
//						char ip[22];
//						std::cout << "\n!~유저 강종(헤더)~! Ip = " << inet_ntop(AF_INET, &(recvIter.m_sockInfo.sin_addr), ip, INET_ADDRSTRLEN)
//								  << ", Port = " << ntohs(recvIter.m_sockInfo.sin_port) << endl;
//						DeleteUser(&recvIter);
//						break;
//					}
//					// 입력 없음
//					break;
//				}
//				// 패킷 메세지
//				while (receiveByte < recvIter.m_packet.header.length)
//				{
//					receiveByte += recv(recvIter.m_socket, (char*)&recvIter.m_packet + receiveByte, recvIter.m_packet.header.length - receiveByte, 0);
//					if (receiveByte == SOCKET_ERROR || receiveByte == 0)
//					{
//						if (WSAGetLastError() != WSAEWOULDBLOCK)
//						{
//							// 에러 -> 유저 내보냄
//							wstringstream ostr;
//							ostr << endl << L"User Receive Error!";
//							OutputDebugString(ostr.str().c_str());
//							char ip[22];
//							std::cout << "\n!~유저 강종(메세지)~! Ip = " << inet_ntop(AF_INET, &(recvIter.m_sockInfo.sin_addr), ip, INET_ADDRSTRLEN)
//									  << ", Port = " << ntohs(recvIter.m_sockInfo.sin_port) << endl;
//							DeleteUser(&recvIter);
//							break;
//						}
//						// 입력 없음
//						break;
//					}
//				}
//			}
//
//			// 패킷 처리
//			if (receiveByte == recvIter.m_packet.header.length)
//			{
//				//memcpy(&recvIter.m_packet, &recvPacket, sizeof(UPACKET));
//				// 이름 없으면 등록
//				if (recvIter.m_userName.empty())
//				{
//					if (auto length = strlen(recvIter.m_packet.msg); recvIter.m_packet.msg[length - 1] == '\n')
//						recvIter.m_packet.msg[length - 1] = '\0';
//					recvIter.m_userName = recvIter.m_packet.msg;
//					//size_t finder = recvIter.m_userName.find('\n');
//					//if (finder != std::string::npos)
//						//recvIter.m_userName.resize(finder);
//
//					SetPacket(&recvIter, (recvIter.m_userName + "님이 입장하셨습니다.\n").c_str(), PACKET_SYSTEM_MSG, true);
//					BroadcastMessage(recvIter, true);
//				}
//				// 리시브 됐다면 샌드
//				else
//				{
//					SetPacket(&recvIter, recvIter.m_packet.msg, recvIter.m_packet.header.type, true);
//					BroadcastMessage(recvIter, false);
//				}
//			}
//		}
//		// 실질적 유저 삭제
//		if (!g_UserDeleteList.empty())
//		{
//			for (auto iter = g_UserDeleteList.begin(); iter != g_UserDeleteList.end();)
//			{
//				auto delIter = find(g_Userlist.begin(), g_Userlist.end(), **iter);
//				g_Userlist.erase(delIter);
//				g_UserDeleteList.erase(iter);
//				iter = g_UserDeleteList.begin();
//			}
//		}
//		LeaveCriticalSection(&g_CriticalSection);
//		Sleep(1);
//	}
//	return 0;
//}
//

int main()
{
	ServerManager& serverManager = ServerManager::GetInstance();
	// 임계구역 설정용
	//InitializeCriticalSection(&g_CriticalSection);
	while (true)
	{
		 // WSA : WindowSocketAPI
		 // ~서버 접속~
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 0;
		USHORT PortNumber = 10000;

		// 내 서버의 소켓,		AF_INET : 주소 패밀리, PF_INET : 프로토콜 패밀리
		SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN socketInfo;
		ZeroMemory(&socketInfo, sizeof(socketInfo));
		socketInfo.sin_family = AF_INET;											// 접속 방식(주소 패밀리)
		// Host 바이트 정렬(little endian) to Network 바이트 정렬(bit endian) type
		socketInfo.sin_port = htons(PortNumber);									// 접속할 서버 포트 번호
		//socketInfo.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");				// 문자 주소를 1바이트씩 끊어 정수 주소로 만듬
		socketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);						// 어떤 주소로 접속하든 포트 번호만 맞으면 받아줌(0)

		int	iRet = bind(listenSock, (SOCKADDR*)&socketInfo, sizeof(socketInfo));	// 소켓을 대상 ip(+포트)에 연결
		if (iRet == SOCKET_ERROR)	return 0;

		iRet = listen(listenSock, SOMAXCONN);
		if (iRet == SOCKET_ERROR)	return 0;
		cout << "서버가 연결되었습니다.\n";
		// 소켓을 논블록킹화, 1 or 0
		ULONG on = true;
		ioctlsocket(listenSock, FIONBIO, &on);

		// 쓰레드 가동
		UINT accepID;
		auto hAccept = _beginthreadex(nullptr, 0, serverManager.AcceptThread, (void*)listenSock, 0, &accepID);
		UINT recvSendID;
		auto hRecvSend = _beginthreadex(nullptr, 0, serverManager.RecvThread, (void*)listenSock, 0, &recvSendID);
		UINT sendSendID;
		auto hSendSend = _beginthreadex(nullptr, 0, serverManager.SendThread, (void*)listenSock, 0, &sendSendID);

		// 끝날때까지 대기
		WaitForSingleObject((HANDLE)hAccept, INFINITE);
		WaitForSingleObject((HANDLE)hRecvSend, INFINITE);
		WaitForSingleObject((HANDLE)hSendSend, INFINITE);
		// 핸들 닫기
		CloseHandle((HANDLE)hAccept);
		CloseHandle((HANDLE)hRecvSend);
		CloseHandle((HANDLE)hSendSend);

		closesocket(listenSock);
		// 접속 종료
		WSACleanup();
	}
	return 0;
}
