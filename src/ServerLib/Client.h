#pragma once
#include "header.h"
#include "ServerBasic.h"


class ClientRecv : public ServerBasic
{
protected:
	WORD& m_MsgType;
	bool Run() override;
public:
	ClientRecv(WORD& msgType) : m_MsgType(msgType) {};
	virtual ~ClientRecv() = default;
};

class ClientSend : public ServerBasic
{
protected:
	WORD& m_MsgType;
	bool Run() override;
public:
	ClientSend(WORD& msgType) : m_MsgType(msgType) {};
	virtual ~ClientSend() = default;
};



class Client
{
protected:
	WORD m_MsgType;
public:
	ClientSend m_Sender;
	ClientRecv m_Recver;

	bool Start(const USHORT& portNum, const string_view& address);

	Client();
	virtual ~Client();
};
