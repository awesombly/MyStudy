#pragma once
#include "../../include/server/ServerBasic.h"
#include "../../include/server/MyProtocol.h"

constexpr WORD MaxBufferSize = 10000;

class DxClientRecv : public ServerBasic
{
protected:
	UPACKET*  m_pUPacket = nullptr;		// 기준 확인용 포인터
	UPACKET*  m_pPacket = nullptr;		// 전송할 패킷
	int		  m_popPos = 0;		// = 버퍼 땡길때 기준
	char	  m_packetBuffer[MaxBufferSize] = { 0, };

	//WSABUF	  m_WSABuffer = { MaxBufferSize, m_packetBuffer };
	int		  m_pushPos = 0;		// = 남은 데이터 양
protected:
	//WORD & m_MsgType;
	bool Run() override;
	void PushData(const int& size) noexcept;
public:
	//DxClientRecv(WORD& msgType) : m_MsgType(msgType) {};
	DxClientRecv() = default;
	virtual ~DxClientRecv() = default;
};

class DxClientSend : public ServerBasic
{
protected:
	//WORD & m_MsgType;
	bool Run() override;
public:
	//DxClientSend(WORD& msgType) : m_MsgType(msgType) {};
	bool Start(const USHORT& portNum, const string_view& address, const bool& isClient = false) override;
	DxClientSend() = default;
	virtual ~DxClientSend() = default;
};
//
//class DxClient
//{
//protected:
//	WORD m_MsgType;
//public:
//	DxClientSend m_Sender;
//	DxClientRecv m_Recver;
//public:
//	bool Start(const USHORT& portNum, const string_view& address);
//	DxClient();
//	virtual ~DxClient();
//};
