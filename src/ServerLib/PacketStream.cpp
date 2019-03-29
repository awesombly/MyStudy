#include "PacketStream.h"
#include "ServerManager.h"
#include "MyPacket.h"


void PacketStream::PushData(const int& size) noexcept
{
	//memcpy(m_packetBuffer + m_pushPos, data, size);
	m_pushPos += size;

	// 헤더를 못받았더라도 기본적으로 0으로 처리되므로 문제 X
	m_pUPacket = (UPACKET*)m_packetBuffer;
	// 남은 데이터가 바꿀 패킷 길이보다 크다면
	while (m_pUPacket->header.length <= m_pushPos)
	{
		// 패킷 완성
		m_pPacket = ServerManager::GetInstance().TakePacket();
		memcpy(&m_pPacket->m_uPacket, m_packetBuffer + m_popPos, m_pUPacket->header.length);
		// 패킷 처리
		ServerManager::Instance->SetPacket(m_pPacket, m_pMyUser);
		// 버퍼 기준 정리
		m_popPos += m_pUPacket->header.length;				// 버퍼 기준점
		m_pushPos -= m_pUPacket->header.length;				// 패킷 길이만큼 감소
		m_pUPacket = (UPACKET*)&m_packetBuffer[m_popPos];	// 다음 패킷 기준
		if (m_pushPos == 0 || m_pUPacket->header.length == 0)
			break;
	}
	// 남은 버퍼 땡기기
	memcpy(m_packetBuffer, m_packetBuffer + m_popPos, MaxBufferSize - m_popPos);
	ZeroMemory(m_packetBuffer + MaxBufferSize - m_popPos, m_popPos);
	m_popPos = 0;
}