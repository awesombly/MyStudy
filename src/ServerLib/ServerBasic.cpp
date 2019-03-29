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
// 쓰레드 구동
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

	// 타 프로세스에서 해당 IP, Port 못쓰게 함 (윈도우)
	//setsockopt(m_ListenSocket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optValue, sizeof(optValue));
	int optValue = 1;
	// (강제종료시)기존에 할당된 소켓 자원을 재사용가능하게 해줌 (리눅스, 유닉스)
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optValue, sizeof(optValue));
	// TCP의 default값인 Nagle 알고리즘(작은 패킷 모아 처리)을 비활성화
	setsockopt(m_ListenSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&optValue, sizeof(optValue));
	// 미전송 패킷 처리 방법(보낼때까지 Blocking)
	linger optLinger;
	optLinger.l_linger = 1000;			// 타임아웃(폐기 카운터)
	optLinger.l_onoff = 1;				// 활성화 여부
	setsockopt(m_ListenSocket, SOL_SOCKET, SO_LINGER, (char*)&optLinger, sizeof(linger));

	SOCKADDR_IN socketInfo = { 0, };
	socketInfo.sin_family = AF_INET;						// 접속 방식
	socketInfo.sin_port = htons(portNum);					// 접속 할 포트 번호
															// ip 설정
	if (address.empty())
		socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, INADDR_ANY, &sa.sin_addr)
	else
		socketInfo.sin_addr.s_addr = inet_addr(address.data());
		//InetPton(AF_INET, address, &sa.sin_addr)

	if (isClient)
	{
		// 서버 접속
		int ret = connect(m_ListenSocket, (const SOCKADDR*)&socketInfo, (int)sizeof(socketInfo));
		if (ret == SOCKET_ERROR)
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Client Connect Error!");
			return false;
		}
	}
	else
	{
		// 소켓을 대상  ip에 연결
		int ret = bind(m_ListenSocket, (const SOCKADDR*)&socketInfo, (int)sizeof(socketInfo));
		if (ret == SOCKET_ERROR)
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Socket Bind Error!");
			return false;
		}
		// 클라이언트의 연결 요청을 확인
		ret = listen(m_ListenSocket, SOMAXCONN);
		if (ret == SOCKET_ERROR)
		{
			Server::ErrorMessage(""s + __FUNCTION__ + " -> Socket Listen Error!");
			return false;
		}
	}
	// 쓰레드 가동
	CreateThread();
	std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	return true;
}

bool ServerBasic::ByteErrorCheck(const int& byte)
{
	if (byte == SOCKET_ERROR || byte == 0)
	{
		// 논블럭일시 에러
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