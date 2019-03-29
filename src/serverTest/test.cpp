#include "headhead.h"
#include "Client.h"


int main()
{
	// WSA : WindowSocketAPI
	// ~辑滚 立加~
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;
	USHORT PortNumber = 10000;

	Acceptor acceptor;
	Sender sender;
	Receiver receiver;
	acceptor.Start(PortNumber, "");
	sender.Start(PortNumber, "");
	receiver.Start(PortNumber, "");

	std::this_thread::sleep_for(std::chrono::seconds(1));
	//Client client;
	//client.Start(PortNumber, "192.168.0.8");

	if(acceptor.m_Thread.joinable())
		acceptor.m_Thread.join();
	if (sender.m_Thread.joinable())
		sender.m_Thread.join();
	if (receiver.m_Thread.joinable())
		receiver.m_Thread.join();
	//WaitForSingleObject((HANDLE)acceptor.m_hThread, INFINITE);
	//WaitForSingleObject((HANDLE)sender.m_hThread, INFINITE);
	//WaitForSingleObject((HANDLE)receiver.m_hThread, INFINITE);
	
	// 立加 辆丰
	WSACleanup();
	return 0;
}