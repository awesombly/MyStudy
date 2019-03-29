#include "Receiver.h"
#include "UserInfo.h"


bool Receiver::Run()
{
	{
		//Synchronize sync(&m_CriSection);
		std::lock_guard<std::mutex> guarder(m_Mutex);
		cout << __FUNCTION__ << " -> ���� �Ǿ����ϴ�.\n";
	}
	bool		retVal = false;
	DWORD		recvByte = 0;
	UserInfo*	pUser = nullptr;
	OVERLAPPED* pOV = nullptr;

	//MyPacket* pRecvPacket = m_serverManager.TakePacket();
	while (m_isCreated)
	{
		// IOCP ť�� ���϶����� ���
		retVal = GetQueuedCompletionStatus(m_serverManager.m_hIOCP, &recvByte, (PULONG_PTR)&pUser, &pOV, INFINITE);
		{
			//Synchronize sync(&m_CriSection);					// ����ȭ ��ü ����
			std::lock_guard<std::mutex> guarder(m_Mutex);
			if (retVal && pUser != nullptr && pOV != nullptr)
			{
				if (ByteErrorCheck(recvByte))
				{
					Server::ErrorMessage(""s + __FUNCTION__ + " -> Recv Byte Error!");
					m_serverManager.DeleteUser(pUser);
				}
				else
				{
					// ��Ŷ, Recv ó��
					pUser->m_packetStream.PushData(recvByte);
					pUser->RecvAsync();
				}
			}
			else
			{
				if (pUser != nullptr && recvByte == 0 && !retVal &&
					GetLastError() != ERROR_OPERATION_ABORTED)
				{
					Server::ErrorMessage(""s + __FUNCTION__ + " -> IOCP Recv Error!");
					m_serverManager.DeleteUser(pUser);
				}
			}
			recvByte = 0;
			pUser = nullptr;
			pOV = nullptr;
			retVal = false;
		}
		//Sleep(1);
		std::this_thread::yield();
	}
	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
	closesocket(m_ListenSocket);
	return true;
}