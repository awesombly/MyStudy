#include <WinSock2.h>
#include <iostream>
#include <process.h>
#pragma comment( lib, "ws2_32.lib")

using namespace std;

int main()
{
	//HANDLE	m_hWriteFile = CreateFile(L"../../sql.iso", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	// WSA : WindowSocketAPI
	// ~���� ����~
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;
	USHORT PortNumber = 10000;

	// �� ������ ����,		AF_INET : �ּ� �йи�, PF_INET : �������� �йи�
	SOCKET listenSock = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN socketInfo;
	ZeroMemory(&socketInfo, sizeof(socketInfo));
	socketInfo.sin_family = AF_INET;											// ���� ���(�ּ� �йи�)
	socketInfo.sin_port = htons(PortNumber);									// ������ ���� ��Ʈ ��ȣ
	socketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);						// � �ּҷ� �����ϵ� ��Ʈ ��ȣ�� ������ �޾���(0)

	int	iRet = bind(listenSock, (SOCKADDR*)&socketInfo, sizeof(socketInfo));	// ������ ��� ip(+��Ʈ)�� ����
	if (iRet == SOCKET_ERROR)	return 0;

	//iRet = listen(listenSock, SOMAXCONN);
	//if (iRet == SOCKET_ERROR)	return 0;
	cout << "������ ����Ǿ����ϴ�.\n";


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
	// ���� ����
	WSACleanup();
	while (true);
	return 0;
}