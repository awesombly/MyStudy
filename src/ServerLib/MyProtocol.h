#pragma once
#include <Windows.h>

//#define PACKET_HEADER_SIZE 4
//#define PACKET_MAX_SIZE    2048
static constexpr WORD PACKET_HEADER_SIZE = 4;
static constexpr WORD PACKET_MAX_SIZE = 2048;

enum EPacket : WORD {
	RANGE_SELF		= 1,
	RANGE_NOTSELF	= 2,

	CHAT_MSG		= 1000,
	CHAT_NAME_REQ	= 2000,
	CHAT_NAME_ACK	= 3000,
	SYSTEM_MSG		= 9000,
};

// ����ü�� 1����Ʈ ������ ��ŷ
#pragma pack(push, 1)
typedef struct {
	WORD length;
	WORD type;
} PACKET_HEADER;

typedef struct {
	PACKET_HEADER header;
	char		  msg[PACKET_MAX_SIZE];
} UPACKET;
// ����ü ��ŷ ����
#pragma pack(pop)

//class Protocol {
//public:
//	struct CharacterInfo {
//		int a;
//		int b;
//		bool c;
//		float d;
//
//	};
//}

//#define CHAT_MSG			1000
//#define CHAT_NAME_REQ	2000
//#define CHAT_NAME_ACK	3000
//#define SYSTEM_MSG		9000
////#define PACKET_RANGE_BROADCAST	0
//#define RANGE_SELF		1		// �ڱ����׸�
//#define RANGE_NOTSELF	2		// �ڱ� ���� ��ε�