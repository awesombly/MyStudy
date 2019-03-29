#include "ServerBasic.h"
#include "ServerManager.h"

//CRITICAL_SECTION SyncParent::m_CriSection;
//SyncParent::SyncParent()
//{
//	InitializeCriticalSection(&m_CriSection);
//}
//SyncParent::~SyncParent()
//{
//	DeleteCriticalSection(&m_CriSection);
//}

std::mutex ServerBasic::m_Mutex;

ServerBasic::~ServerBasic()
{
	//CloseHandle((HANDLE)m_hThread);
	//m_Thread.native_handle();
}


void ServerBasic::CreateThread()
{
	if (m_isCreated)	return;
	m_isCreated = true;
	//m_hThread = _beginthreadex(nullptr, 0, ServerBasic::Runner, (void*)this, 0, &m_ThreadID);
	m_Thread = std::thread(ServerBasic::Runner, this);
}
// ������ ����
UINT WINAPI ServerBasic::Runner(LPVOID arg)
{
	ServerBasic* pServer = (ServerBasic*)arg;
	pServer->Run();
	return 0;
}

bool ServerBasic::Start(const USHORT& portNum, const string_view& address, const bool& isClient)
{
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ListenSocket == INVALID_SOCKET)
	{
		Server::ErrorMessage(""s + __FUNCTION__ + " -> Socket Init Error!");
		return false;
	}

	// Ÿ ���μ������� �ش� IP, Port ������ �� (������)
	//setsockopt(m_ListenSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optValue, sizeof(optValue));
	int optValue = 1;
	// (���������)������ �Ҵ�� ���� �ڿ��� ���밡���ϰ� ���� (������, ���н�)
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optValue, sizeof(optValue));
	// TCP�� default���� Nagle �˰���(���� ��Ŷ ��� ó��)�� ��Ȱ��ȭ
	setsockopt(m_ListenSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&optValue, sizeof(optValue));
	// ������ ��Ŷ ó�� ���(���������� Blocking)
	linger optLinger;
	optLinger.l_linger = 1000;			// Ÿ�Ӿƿ�(��� ī����)
	optLinger.l_onoff = 1;				// Ȱ��ȭ ����
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_LINGER, (char*)&optLinger, sizeof(linger));

	SOCKADDR_IN socketInfo = { 0, };
	socketInfo.sin_family = AF_INET;						// ���� ���
	socketInfo.sin_port = htons(portNum);					// ���� �� ��Ʈ ��ȣ
															// ip ����
	if (address.empty())
		socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, INADDR_ANY, &sa.sin_addr)
	else
		socketInfo.sin_addr.s_addr = inet_addr(address.data());
		//InetPton(AF_INET, address, &sa.sin_addr)

	if (isClient)
	{
		// ���� ����
		int ret = connect(m_ListenSocket, (const SOCKADDR*)&socketInfo, (int)sizeof(socketInfo));
		if (ret == SOCKET_ERROR)
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Client Connect Error!");
			return false;
		}
	}
	else
	{
		// ������ ���  ip�� ����
		int ret = bind(m_ListenSocket, (const SOCKADDR*)&socketInfo, (int)sizeof(socketInfo));
		if (ret == SOCKET_ERROR)
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Socket Bind Error!");
			return false;
		}
		// Ŭ���̾�Ʈ�� ���� ��û�� Ȯ��
		ret = listen(m_ListenSocket, SOMAXCONN);
		if (ret == SOCKET_ERROR)
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Socket Listen Error!");
			return false;
		}
	}
	// ������ ����
	CreateThread();
	std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	return true;
}

bool ServerBasic::ByteErrorCheck(const int& byte)
{
	if (byte == SOCKET_ERROR || byte == 0)
	{
		// ����Ͻ� ����
		//if (WSAGetLastError() != WSAEWOULDBLOCK)
		//{
		return true;
		//}
	}
	return false;
}



//Synchronize::Synchronize(CRITICAL_SECTION* criSection)
//{
//	m_pCriSection = criSection;
//	EnterCriticalSection(m_pCriSection);
//}
//Synchronize::~Synchronize()
//{
//	LeaveCriticalSection(m_pCriSection);
//}