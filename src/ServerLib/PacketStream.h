#pragma once
#include "header.h"
#include "MyPacket.h"

constexpr WORD MaxBufferSize = 10000;

class UserInfo;

class PacketStream
{
protected:
	MyPacket* m_pPacket = nullptr;
	UPACKET*  m_pUPacket = nullptr;
	int		  m_popPos  = 0;		// = 버퍼 땡길때 기준
	char	  m_packetBuffer[MaxBufferSize] = { 0, };
public:
	WSABUF	  m_WSABuffer = { MaxBufferSize, m_packetBuffer };
	UserInfo* m_pMyUser = nullptr;
	int		  m_pushPos = 0;		// = 남은 데이터 양
public:
	void PushData(const int& size) noexcept;

	PacketStream() noexcept = default;
	virtual ~PacketStream() noexcept = default;
};