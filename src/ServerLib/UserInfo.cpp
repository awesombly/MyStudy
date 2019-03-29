#include "UserInfo.h"
#include "ServerManager.h"

UserInfo::UserInfo()
{
	m_packetStream.m_pMyUser = this;
	//m_recvThread.m_pUser = this;
	//m_recvThread.Start(10000, nullptr);
}

void UserInfo::Init()
{
	ZeroMemory(&m_sockInfo, sizeof(m_sockInfo));
	m_socket = 0;
	m_userName.clear();
	m_packetStream.m_pMyUser = this;
	//m_recvThread.m_pUser = this;
	//m_recvThread.m_isCreated = false;
	//m_recvThread.Start(10000, nullptr);
}

void UserInfo::Release()
{
	shutdown(m_socket, SD_SEND);
	closesocket(m_socket);									// 소켓 닫기
	//pDelUser->m_recvThread.m_isCreated = false;			// 쓰레드 종료
	ServerManager::Instance->m_DisableUserPool.push(this);	// 대기 풀 입력
}

void UserInfo::RecvAsync()
{
	static int retVal = 0;
	static DWORD recvLength = 0;
	static DWORD flag = 0;
	static OVERLAPPED ov{ 0, };

	// IOCP 핸들-유저 바인딩
	CreateIoCompletionPort((HANDLE)m_socket, ServerManager::Instance->m_hIOCP, (ULONG_PTR)this, 0);
	// 비동기 Recv
	retVal = WSARecv(m_socket, &m_packetStream.m_WSABuffer, 1,
					 &recvLength, &flag, &ov, nullptr);
	if (retVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		Server::ErrorMessage(""s + __FUNCTION__ + " -> IOCP Binding Error!");
	}
}

bool operator == (const UserInfo* pUser, const UserInfo& pUser2)
{
	return pUser == &pUser2;
}



bool UserThread::Run()
{
	//MyPacket* pRecvPacket = m_serverManager.TakePacket();

	while (m_isCreated)
	{
		// 헤더 만들기
		int receiveByte = 0;
		// 패킷 데이터 리시브
		receiveByte = recv(m_pUser->m_socket, (char*)&m_pUser->m_packetStream.m_WSABuffer.buf + m_pUser->m_packetStream.m_pushPos, MaxBufferSize / 2, 0);
		{
			//Synchronize sync(&m_CriSection);					// 동기화 객체 선언
			std::lock_guard<std::mutex> guarder(m_Mutex);
			if(ByteErrorCheck(receiveByte))
			{
				// 에러 -> 유저 내보냄
				Server::ErrorMessage(""s + __FUNCTION__ + " -> User Exit : " + m_pUser->m_userName.c_str());
				m_serverManager.DeleteUser(m_pUser);
				receiveByte = 0;
				break;
			}
			// 패킷 처리
			m_pUser->m_packetStream.PushData(receiveByte);
		}
		//Sleep(1);
		//std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		std::this_thread::yield();
	}

	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	closesocket(m_ListenSocket);
	return true;
}