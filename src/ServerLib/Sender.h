#pragma once
#include "ServerBasic.h"

class Sender : public ServerBasic
{
protected:
	virtual bool Run() override;
public:
	void ProcessPacket(MyPacket* myPacket);
	void SendPacket(const UserInfo& target, const MyPacket& packet);
	void BroadcastMessage(const MyPacket& packet, const bool& sendSelf);

	Sender() = default;
	virtual ~Sender() = default;
};