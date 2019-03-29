#include "ServerManager.h"
#include "MyPacket.h"
#include "UserInfo.h"

ServerManager::ServerManager()
{
	// IOCP �ڵ� ����
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

MyPacket* ServerManager::TakePacket()
{
	MyPacket* pPacket = nullptr;
	if (m_DisablePacketPool.empty())
	{
		// ��� Ǯ�� ����ٸ� ����
		pPacket = new MyPacket();
	}
	else
	{
		// ��� Ǯ�� �ִٸ� ������
		pPacket = m_DisablePacketPool.top();
		m_DisablePacketPool.pop();
		ZeroMemory(pPacket, sizeof(*pPacket));
	}
	return pPacket;
}

UserInfo* ServerManager::TakeUserInfo()
{
	UserInfo* pUserInfo = nullptr;
	if (m_DisableUserPool.empty())
	{
		// ��� Ǯ�� ����ٸ� ����
		pUserInfo = new UserInfo();
	}
	else
	{
		// ��� Ǯ�� �ִٸ� ������
		pUserInfo = m_DisableUserPool.top();
		m_DisableUserPool.pop();
		pUserInfo->Init();
	}
	return pUserInfo;
}

void ServerManager::SetPacket(MyPacket* pPacket, UserInfo* pUserInfo, const WORD& flag, const bool& printServer)
{
	SetPacket(pPacket, pUserInfo, pPacket->m_uPacket.msg, pPacket->m_uPacket.header.type, flag, printServer);
}

void ServerManager::SetPacket(MyPacket* pPacket , UserInfo* pUserInfo, const string_view& msg, const WORD& msgType, const WORD& flag, const bool& printServer)
{
	pPacket->m_uPacket.header.type = msgType;
	pPacket->m_pMyUser = pUserInfo;

	static string buffer;
	switch (msgType)
	{
	case EPacket::CHAT_MSG:
	{
		buffer = pUserInfo->m_userName + " : " + msg.data();
		//pPacket->m_uPacket.header.type |= EPacket::RANGE_NOTSELF;
	}	break;
	case EPacket::CHAT_NAME_ACK:
	{
		pPacket->m_uPacket.header.type = EPacket::CHAT_MSG;
		// ���� ����
		if (auto length = strlen(pPacket->m_uPacket.msg);
		msg.at(length - 1) == '\n')
			const_cast<string_view&>(msg).remove_suffix(1);
		pUserInfo->m_userName = msg;
		buffer = "[" + pUserInfo->m_userName + "]" + "���� �����ϼ̽��ϴ�.\n";
		auto iter = find(m_AcceptUserList.begin(), m_AcceptUserList.end(), pUserInfo);
		if (iter == m_AcceptUserList.end())
			break;
		m_UserList.push_back(*iter);
		m_AcceptUserList.erase(iter);
	}	break;
	case EPacket::CHAT_NAME_REQ:
	{
		buffer = msg;
	}	break;
	case EPacket::SYSTEM_MSG:
	{
		pPacket->m_uPacket.header.type = EPacket::CHAT_MSG;
		buffer = msg;
	}	break;
	default:
	{
		Server::ErrorMessage(""s + __FUNCTION__ + " -> " + pUserInfo->m_userName + " : MSG Type Not Found!");
		buffer = msg;
	}	break;
	}
	// �޼���, ����
	memcpy(pPacket->m_uPacket.msg, buffer.c_str(), strlen(buffer.c_str()));
	if (auto length = strlen(pPacket->m_uPacket.msg); 
		pPacket->m_uPacket.msg[length - 1] != '\n')
		pPacket->m_uPacket.msg[length]		= '\n';
	pPacket->m_uPacket.header.length = WORD(strlen(pPacket->m_uPacket.msg) + PACKET_HEADER_SIZE);

	pPacket->m_uPacket.header.type |= flag;
	if (printServer)	cout << pPacket->m_uPacket.msg;
	// ��ŶǮ�� ��Ŷ �߰�
	m_PacketPool.push(pPacket);
}


void ServerManager::DeleteUser(UserInfo* pDelUser)
{
	static char ip[22] = { 0, };
	cout << "\n!~���� ����~! Ip = " << inet_ntop(AF_INET, &(pDelUser->m_sockInfo.sin_addr), ip, INET_ADDRSTRLEN)
		<< ", Port = " << ntohs(pDelUser->m_sockInfo.sin_port) << endl;
	auto iter = find(m_UserList.begin(), m_UserList.end(), pDelUser);
	if (iter == m_UserList.end())
	{
		iter = find(m_AcceptUserList.begin(), m_AcceptUserList.end(), pDelUser);
		if (iter == m_AcceptUserList.end())
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> " + pDelUser->m_userName.c_str() + " : UserInfo Not Found!");
			return;
		}
		SetPacket(TakePacket(), pDelUser, ("������ ���Դ� �������ϴ�.\n"), EPacket::SYSTEM_MSG, EPacket::RANGE_NOTSELF);
		pDelUser->Release();
		m_AcceptUserList.erase(iter);
		return;
	}
	SetPacket(TakePacket(), pDelUser, "[" + pDelUser->m_userName + "]���� �����̽��ϴ�.\n", EPacket::SYSTEM_MSG, EPacket::RANGE_NOTSELF);
	pDelUser->Release();
	m_UserList.erase(iter);
}