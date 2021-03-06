#pragma once
#include "../../include/server/ServerBasic.h"
#include "../../include/server/MyProtocol.h"
//class UPACKET;

class Worker :public ServerBasic
{
public:
protected:
	virtual bool Run() override;
public:
	bool Start(const USHORT& portNum, const string_view& address, const bool& isClient = false) override;
	void ProcessPacket(UPACKET* myPacket);

	Worker() = default;
	virtual ~Worker() = default;
};

