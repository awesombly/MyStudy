#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <process.h>
#include <iostream>
#pragma comment( lib, "ws2_32.lib")

using namespace std;

int main()
{
	//HANDLE	m_hWriteFile = CreateFile(L"../../123.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	// WSA : WindowSocketAPI
	// ~서버 접속~
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;
	USHORT PortNumber = 10000;
	int	iRet = 0;

	// 내 서버의 소켓,		AF_INET : 주소 패밀리, PF_INET : 프로토콜 패밀리
	SOCKET listenSock = socket(AF_INET, SOCK_DGRAM, 0);

	bool optval = true;
	iRet = setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)	return 0;

	SOCKADDR_IN socketInfo{ 0, };
	//ZeroMemory(&socketInfo, sizeof(socketInfo));
	socketInfo.sin_family = AF_INET;											// 접속 방식(주소 패밀리)
	socketInfo.sin_port = htons(PortNumber);									// 접속할 서버 포트 번호
	socketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);						// 어떤 주소로 접속하든 포트 번호만 맞으면 받아줌(0)

	iRet = bind(listenSock, (SOCKADDR*)&socketInfo, sizeof(socketInfo));		// 소켓을 대상 ip(+포트)에 연결
	if (iRet == SOCKET_ERROR)	return 0;

	//ip_mreq_source mreqS;
	//mreqS.imr_sourceaddr = inet_addr("192.168.0.0");
	// 멤버쉽 가입 : 해당 멤버쉽 멤버와 모두 연결
	// 소스 멤버쉽 : 해당 유저를 지정해 연결, 해제
	ip_mreq mreq{ 0, };
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);								// 내 주소
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");							// 가입 주소
	// 멤버쉽 가입
	setsockopt(listenSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

	//iRet = listen(listenSock, SOMAXCONN);
	//if (iRet == SOCKET_ERROR)	return 0;
	cout << "서버가 연결되었습니다.\n";
	// &RecvInfo
	SOCKADDR_IN peeraddr{ 0, };
	constexpr UINT size = 1024 * 30;
	char buf[size] = { 0, };
	int addrlen = sizeof(peeraddr);
	DWORD writeByte = 0;
	// SendInfo
	SOCKADDR_IN addr{ 0, };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PortNumber);
	addr.sin_addr.s_addr = inet_addr("235.7.8.9");
	while (1)
	{
		char buf2[] = "asdf1234";
		iRet = sendto(listenSock, buf2, (int)strlen(buf2), 0, (SOCKADDR*)&addr, (int)sizeof(addr));

		iRet = recvfrom(listenSock, buf, size, 0, (SOCKADDR*)&peeraddr, &addrlen);
		buf[iRet] = 0;
		cout << buf << endl;
		Sleep(1000);
		//if (strcmp(buf, "end") == 0) break;
		//WriteFile(m_hWriteFile, buf, ret, &writeByte, nullptr);
	}
	//CloseHandle(m_hWriteFile);
	closesocket(listenSock);
	// 접속 종료
	WSACleanup();
	return 0;
}