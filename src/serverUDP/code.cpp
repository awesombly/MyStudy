#include <WinSock2.h>
#include <iostream>
#include <process.h>
#pragma comment( lib, "ws2_32.lib")

using namespace std;

int main()
{
	//HANDLE	m_hWriteFile = CreateFile(L"../../sql.iso", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	// WSA : WindowSocketAPI
	// ~서버 접속~
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;
	USHORT PortNumber = 10000;

	// 내 서버의 소켓,		AF_INET : 주소 패밀리, PF_INET : 프로토콜 패밀리
	SOCKET listenSock = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN socketInfo;
	ZeroMemory(&socketInfo, sizeof(socketInfo));
	socketInfo.sin_family = AF_INET;											// 접속 방식(주소 패밀리)
	socketInfo.sin_port = htons(PortNumber);									// 접속할 서버 포트 번호
	socketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);						// 어떤 주소로 접속하든 포트 번호만 맞으면 받아줌(0)

	int	iRet = bind(listenSock, (SOCKADDR*)&socketInfo, sizeof(socketInfo));	// 소켓을 대상 ip(+포트)에 연결
	if (iRet == SOCKET_ERROR)	return 0;

	//iRet = listen(listenSock, SOMAXCONN);
	//if (iRet == SOCKET_ERROR)	return 0;
	cout << "서버가 연결되었습니다.\n";


	SOCKADDR_IN peeraddr;
	constexpr UINT size = 4096 * 16;
	char buf[size] = { 0, };
	int addrlen = sizeof(peeraddr);

	DWORD writeByte = 0;
	while (1)
	{
		int ret = recvfrom(listenSock, buf, size, 0, (SOCKADDR*)&peeraddr, &addrlen);
		buf[ret] = 0;
		cout << buf << endl;

		//if (strcmp(buf, "end") == 0) break;
		//WriteFile(m_hWriteFile, buf, ret, &writeByte, nullptr);
	}
	//CloseHandle(m_hWriteFile);
	closesocket(listenSock);
	// 접속 종료
	WSACleanup();
	while (true);
	return 0;
}