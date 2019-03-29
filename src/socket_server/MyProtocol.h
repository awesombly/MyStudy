#pragma once
#include <Windows.h>

#define PACKET_HEADER_SIZE 4
#define PACKET_MAX_SIZE    2048

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

#define PACKET_CHAT_MSG			1000
#define PACKET_CHAT_NAME_REQ	2000
#define PACKET_CHAT_NAME_ACK	3000
#define PACKET_SYSTEM_MSG		9000

//#define PACKET_RANGE_BROADCAST	0
#define PACKET_RANGE_SELF		1
#define PACKET_RANGE_NOTSELF	2