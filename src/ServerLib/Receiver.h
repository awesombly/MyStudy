#pragma once
#include "ServerBasic.h"

class Receiver : public ServerBasic
{
protected:
	virtual bool Run() override;
public:
	Receiver() = default;
	virtual ~Receiver() = default;
};
