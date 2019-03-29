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
	// ~���� ����~
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;
	USHORT PortNumber = 10000;
	int	iRet = 0;

	// �� ������ ����,		AF_INET : �ּ� �йи�, PF_INET : �������� �йи�
	SOCKET listenSock = socket(AF_INET, SOCK_DGRAM, 0);

	bool optval = true;
	iRet = setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)	return 0;

	SOCKADDR_IN socketInfo{ 0, };
	//ZeroMemory(&socketInfo, sizeof(socketInfo));
	socketInfo.sin_family = AF_INET;											// ���� ���(�ּ� �йи�)
	socketInfo.sin_port = htons(PortNumber);									// ������ ���� ��Ʈ ��ȣ
	socketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);						// � �ּҷ� �����ϵ� ��Ʈ ��ȣ�� ������ �޾���(0)

	iRet = bind(listenSock, (SOCKADDR*)&socketInfo, sizeof(socketInfo));		// ������ ��� ip(+��Ʈ)�� ����
	if (iRet == SOCKET_ERROR)	return 0;

	//ip_mreq_source mreqS;
	//mreqS.imr_sourceaddr = inet_addr("192.168.0.0");
	// ����� ���� : �ش� ����� ����� ��� ����
	// �ҽ� ����� : �ش� ������ ������ ����, ����
	ip_mreq mreq{ 0, };
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);								// �� �ּ�
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");							// ���� �ּ�
	// ����� ����
	setsockopt(listenSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

	//iRet = listen(listenSock, SOMAXCONN);
	//if (iRet == SOCKET_ERROR)	return 0;
	cout << "������ ����Ǿ����ϴ�.\n";
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
	// ���� ����
	WSACleanup();
	return 0;
}