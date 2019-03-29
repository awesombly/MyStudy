#pragma once
//#include "UserInfo.h"
#include "MyProtocol.h"

class UserInfo;

class MyPacket
{
protected:
	char*		m_PopPosition;
public:
	UPACKET		m_uPacket;
	UserInfo*	m_pMyUser;
public:
	template <typename DataType>
	MyPacket& operator << (const DataType& data) noexcept;
	template <typename DataType>
	MyPacket& operator >> (DataType* data) noexcept;
	template <typename DataType>
	MyPacket& operator << (const char* data) noexcept;
	template <typename DataType>
	MyPacket& operator >> (char* data) noexcept;
public:
	MyPacket() noexcept : m_uPacket(), m_PopPosition((char*)this), m_pMyUser(nullptr)
	{
		m_PopPosition = (char*)this->m_uPacket.msg;
	}
	virtual ~MyPacket() noexcept = default;
};


template <typename DataType>
MyPacket& MyPacket::operator << (const DataType& data) noexcept
{
	memcpy((char*)&m_uPacket.msg + m_uPacket.header.length, &data, sizeof(DataType));
	m_uPacket.header.length += sizeof(DataType);
	return *this;
}

template <typename DataType>
MyPacket& MyPacket::operator >> (DataType* data) noexcept
{
	memcpy(data, m_PopPosition, sizeof(DataType));
	m_PopPosition += sizeof(DataType);
	return *this;
}

template <typename DataType>
MyPacket& MyPacket::operator << (const char* data) noexcept
{
	memcpy((char*)&m_uPacket.msg + m_uPacket.header.length, &data, strlen(data));
	m_uPacket.header.length += strlen(data);
	return *this;
}

template <typename DataType>
MyPacket& MyPacket::operator >> (char* data) noexcept
{
	memcpy(data, m_PopPosition, strlen(data));
	m_PopPosition += strlen(data);
	return *this;
}