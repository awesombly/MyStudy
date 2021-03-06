// socket_client.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MyProtocol.h"

UINT g_msgType = EPacket::CHAT_MSG;

unsigned int WINAPI SendThread(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;

	UPACKET sendPacket;
	while (true)
	{
		ZeroMemory(&sendPacket, sizeof(UPACKET));

		cout << "\n";
		fgets(sendPacket.msg, sizeof(sendPacket.msg), stdin);
		//getchar();
		if (sendPacket.msg[0] == '`')
			break;

		sendPacket.header.length = (WORD)(strlen(sendPacket.msg) + PACKET_HEADER_SIZE);
		sendPacket.header.type = g_msgType;

		// 샌드->서버
		int sendByte = send(clientSock, (char*)&sendPacket, sendPacket.header.length, 0);
		if (sendByte == SOCKET_ERROR || sendByte == 0)
		{
			//if (WSAGetLastError() != WSAEWOULDBLOCK)
			//{
				wstringstream ostr;
				ostr << endl << L"Server Send Error!";
				OutputDebugString(ostr.str().c_str());
				break;
			//}
			// 입력 없음
		} 
		else if(sendByte > 0)
		{
			cout << sendByte << " 바이트를 전송했다.";
			g_msgType = EPacket::CHAT_MSG;					// 기본값으로 설정
		}
	}
	shutdown(clientSock, SD_SEND);
	closesocket(clientSock);
	return 0;
}

unsigned int WINAPI RecvThread(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;

	UPACKET recvPacket;
	while (true)
	{
		// 서버->리시브
		ZeroMemory(&recvPacket, sizeof(UPACKET));

		// 헤더 만들기
		int receiveByte = 0;
		while (receiveByte < PACKET_HEADER_SIZE)
		{
			receiveByte += recv(clientSock, (char*)&recvPacket + receiveByte, PACKET_HEADER_SIZE - receiveByte, 0);
			if (receiveByte == SOCKET_ERROR || receiveByte == 0)
			{
				//if (WSAGetLastError() != WSAEWOULDBLOCK)
				//{
				// ~~에러~~
				wstringstream ostr;
				ostr << endl << L"Header Recive Error!";
				OutputDebugString(ostr.str().c_str());
				cout << "\n접속 종료\n";
				closesocket(clientSock);
				return 0;
				//}
				// 입력 없음
			}
		}
		//UPACKET* recvPacket = (UPACKET*)&recvBuffer.at(0);
		//receiveByte = 0;
		// 메세지 만들기
		while (receiveByte < recvPacket.header.length)
		{
			receiveByte += recv(clientSock, (char*)&recvPacket + receiveByte, recvPacket.header.length - receiveByte, 0);
			if (receiveByte == SOCKET_ERROR || receiveByte == 0)
			{
				//if (WSAGetLastError() != WSAEWOULDBLOCK)
				//{
				// ~~에러~~
				wstringstream ostr;
				ostr << endl << L"Message Recive Error!";
				OutputDebugString(ostr.str().c_str());
				cout << "\n접속 종료\n";
				closesocket(clientSock);
				return 0;
				//}
				// 입력 없음
			}
		}

		// 패킷 완성 처리
		if (receiveByte == recvPacket.header.length)
		{
			switch (recvPacket.header.type)
			{
			case EPacket::CHAT_MSG:
			{
				//g_msgType = CHAT_MSG;
			}	break;
			case EPacket::CHAT_NAME_REQ:
			{
				g_msgType = EPacket::CHAT_NAME_ACK;
			}	break;
			default:
			{
				stringstream ostr;
				ostr << endl << __FUNCTION__ << " -> " << recvPacket.header.type << ", MSG Type Not Found!";
				OutputDebugStringA(ostr.str().c_str());
			}	break;
			}
			// 받은 데이터 출력, 줄바꿈 없으면 넣어줌
			if (auto length = strlen(recvPacket.msg);  recvPacket.msg[length - 1] != '\n')
				recvPacket.msg[length] = '\n';
			cout << recvPacket.msg;
		}
	}
	shutdown(clientSock, SD_SEND);
	closesocket(clientSock);
	return 0;
}


int main()
{
	// ~클라이언트 접속~
	while (true)
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
		SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN sa;
		ZeroMemory(&sa, sizeof(sa));
		sa.sin_family = AF_INET;									// 접속 방식

		cout << "접속할 ip 끝자리 입력 : ";
		char lastIp[4] = { 0, };
		cin >> lastIp;
		getchar();
		string ipAddr = "192.168.0."s + lastIp;
		sa.sin_addr.S_un.S_addr = inet_addr(ipAddr.c_str());		// 문자 주소를 1바이트씩 끊어 정수 주소로 만듬
		sa.sin_port = htons(10000);									// 접속할 서버 포트 번호

		int iRet = connect(clientSock, (SOCKADDR*)&sa, sizeof(sa));	// 서버 접속
		if (iRet == SOCKET_ERROR) return 0;
		cout << "\n~서버 접속 성공~";

		// 소켓-논블록화
		//ULONG on = true;
		//ioctlsocket(clientSock, FIONBIO, &on);

		// 쓰레드 가동
		UINT recvThreadId;
		UINT sendThreadId;
		uintptr_t hSendThread = _beginthreadex(nullptr, 0, SendThread, (void*)clientSock, 0, &sendThreadId);
		uintptr_t  hRecvThread = _beginthreadex(nullptr, 0, RecvThread, (void*)clientSock, 0, &recvThreadId);
		// 끝날때까지 대기
		WaitForSingleObject((HANDLE)hSendThread, INFINITE);
		WaitForSingleObject((HANDLE)hRecvThread, INFINITE);
		// 핸들 닫기
		CloseHandle((HANDLE)hSendThread);
		CloseHandle((HANDLE)hRecvThread);

		// 접속 종료
		iRet = WSACleanup();
	}
	return 0;
}