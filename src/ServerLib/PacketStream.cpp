#include "PacketStream.h"
#include "ServerManager.h"
#include "MyPacket.h"


void PacketStream::PushData(const int& size) noexcept
{
	//memcpy(m_packetBuffer + m_pushPos, data, size);
	m_pushPos += size;

	// ����� ���޾Ҵ��� �⺻������ 0���� ó���ǹǷ� ���� X
	m_pUPacket = (UPACKET*)m_packetBuffer;
	// ���� �����Ͱ� �ٲ� ��Ŷ ���̺��� ũ�ٸ�
	while (m_pUPacket->header.length <= m_pushPos)
	{
		// ��Ŷ �ϼ�
		m_pPacket = ServerManager::GetInstance().TakePacket();
		memcpy(&m_pPacket->m_uPacket, m_packetBuffer + m_popPos, m_pUPacket->header.length);
		// ��Ŷ ó��
		ServerManager::Instance->SetPacket(m_pPacket, m_pMyUser);
		// ���� ���� ����
		m_popPos += m_pUPacket->header.length;				// ���� ������
		m_pushPos -= m_pUPacket->header.length;				// ��Ŷ ���̸�ŭ ����
		m_pUPacket = (UPACKET*)&m_packetBuffer[m_popPos];	// ���� ��Ŷ ����
		if (m_pushPos == 0 || m_pUPacket->header.length == 0)
			break;
	}
	// ���� ���� �����
	memcpy(m_packetBuffer, m_packetBuffer + m_popPos, MaxBufferSize - m_popPos);
	ZeroMemory(m_packetBuffer + MaxBufferSize - m_popPos, m_popPos);
	m_popPos = 0;
}