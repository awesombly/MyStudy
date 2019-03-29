#include "Acceptor.h"
#include "UserInfo.h"



bool Acceptor::Run()
{
	{
		//Synchronize sync(&m_CriSection);
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> 연결 되었습니다.\n";
	}
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	while (m_isCreated)
	{
		// 클라 접속 허가
		clientSock = accept(m_ListenSocket, (SOCKADDR*)&clientAddr, &addrLength);
		{
			//Synchronize sync(&m_CriSection);					// 동기화 객체 선언
			std::lock_guard<std::mutex> guarder(m_Mutex);
			if (clientSock == SOCKET_ERROR)
			{
				Server::ErrorMessage(""s + __FUNCTION__ + " -> Client Accept Error!");
				break;
			}
			else
			{
				// 접속시 유저 등록
				UserInfo* pUser = m_serverManager.TakeUserInfo();
				pUser->m_socket = clientSock;
				pUser->m_sockInfo = clientAddr;
				m_serverManager.m_AcceptUserList.push_back(pUser);
				std::cout << "~클라이언트 접속~ IP : " << inet_ntoa(pUser->m_sockInfo.sin_addr) << ", Port : " << ntohs(pUser->m_sockInfo.sin_port) << endl;
				m_serverManager.SetPacket(m_serverManager.TakePacket(), pUser, "ㅎㅇㅇ~! 아이디를 입력해 주세용.\n", CHAT_NAME_REQ, RANGE_SELF, false);
				// 비동기 Recv 작업
				CreateIoCompletionPort((HANDLE)pUser->m_socket, m_serverManager.m_hIOCP, (ULONG_PTR)pUser, 0);
				pUser->RecvAsync();
			}
		}
		//Sleep(1);
		std::this_thread::yield();
	}
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	closesocket(m_ListenSocket);
	return true;
}